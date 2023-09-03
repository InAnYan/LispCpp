#include "Object.hpp"

#include <iostream>

#include "Configuration.hpp"

#include "MemoryManager.hpp"

namespace Lisp
{
    Object::Object(GcPtr<Object> next, ObjectType type)
        : isMarked(false), type(type), next(next)
    {
            
    }

    ObjectType Object::GetType() const
    {
        return type;
    }

    bool Object::Is(ObjectType type) const
    {
        return GetType() == type;
    }
    
    GcPtr<Object> Object::GetNext()
    {
        return next;
    }

    void Object::SetNext(GcPtr<Object> newNext)
    {
        next = newNext;
    }

    void Object::Mark()
    {
        if (!isMarked)
        {
            if (Configuration::GC::LOG)
            {
                MemoryManager::LogObject("Mark", GcPtr<Object>(this));
            }

            isMarked = true;
            MarkChildren();
        }
    }

    bool Object::IsMarked() const
    {
        return isMarked;
    }

    void Object::Unmark()
    {
        isMarked = false;
    }

    std::ostream& operator<<(std::ostream& out, const Object& obj)
    {
        return obj.Print(out);
    }
}
