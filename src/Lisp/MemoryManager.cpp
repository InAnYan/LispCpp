#include "MemoryManager.hpp"

#include "Context.hpp"

namespace Lisp
{
    MemoryManager::MemoryManager()
        : objects(GcPtr<Object>(nullptr)), objectsCount(0), oldObjectsCount(0), nextGC(42)
    {
        
    }

    MemoryManager::~MemoryManager()
    {
        GcPtr<Object> obj = objects;
        while (obj)
        {
            GcPtr<Object> next = obj->GetNext();
            DeleteObject(obj);
            obj = next;
        }
    }

    void MemoryManager::CollectGarbageIfNeeded(Context& context, Value value, Value lexicalEnv)
    {
        if (Configuration::GC::STRESS)
        {
            CollectGarbage(context, value, lexicalEnv);
        }
        else if (ShouldDoGarbageCollection())
        {
            CollectGarbage(context, value, lexicalEnv);
        }
    }

    bool MemoryManager::ShouldDoGarbageCollection() const
    {
        return objectsCount >= nextGC;
    }
    void MemoryManager::UpdateNextGC()
    {
        nextGC *= 2;
    }

    void MemoryManager::CollectGarbage(Context& context, Value value, Value lexicalEnv)
    {
        if (Configuration::GC::LOG)
        {
            LogBegin();
        }

        MarkRoots(context, value, lexicalEnv);
        Sweep();

        UpdateNextGC();

        if (Configuration::GC::LOG)
        {
            LogEnd();
        }
    }

    void MemoryManager::PushProtect(GcPtr<Object> obj)
    {
        if (Configuration::GC::LOG)
        {
            LogObject("PushProtect", obj);
        }

        protectStack.push_back(obj);
    }

    void MemoryManager::PopProtect()
    {
        assert(!protectStack.empty());

        if (Configuration::GC::LOG)
        {
            LogObject("PopProtect", protectStack.back());
        }

        protectStack.pop_back();
    }

    void MemoryManager::MarkRoots(Context& context, Value value, Value lexicalEnv)
    {
        MarkValue(value);
        MarkValue(lexicalEnv);

        for (auto obj : protectStack)
        {
            obj->Mark();
        }

        context.GetStringsInterner().MarkObjects();
        context.GetSymbolsInterner().MarkObjects();
        MarkValue(context.GetStackTrace());
    }

    void MemoryManager::Sweep()
    {
        if (Configuration::GC::LOG)
        {
            LogAction("Sweep");
        }

        GcPtr<Object> prev;
        GcPtr<Object> obj = objects;

        while (obj)
        {
            GcPtr<Object> next = obj->GetNext();

            if (obj->IsMarked())
            {
                obj->Unmark();
                prev = obj;
                obj = next;
            }
            else
            {
                if (!prev)
                {
                    objects = next;
                }
                else
                {
                    prev->SetNext(next);
                }

                DeleteObject(obj);
                
                obj = next;
            }
        }
    }

    void MemoryManager::DeleteObject(GcPtr<Object> obj)
    {
        if (Configuration::GC::LOG)
        {
            LogObject("DeleteObject", obj);
        }

        obj.Delete();
    }

    void MemoryManager::LogBegin()
    {
        std::cout << "--- GC: Begin (" << objectsCount << ")" << std::endl;
        oldObjectsCount = objectsCount;
    }

    void MemoryManager::LogEnd()
    {
        std::cout << "--- GC: End (" << oldObjectsCount << "-> " << objectsCount << ", -" << (oldObjectsCount - objectsCount) << ")" << std::endl;
    }

    void MemoryManager::LogObject(const char* action, GcPtr<Object> obj)
    {
        std::cout << "- GC: " << action << ' ';
        obj.PrintPointerValue(std::cout);
        std::cout << std::endl;
    }

    void MemoryManager::LogAction(const char* action)
    {
        std::cout << "- GC: " << action << std::endl;
    }
}
