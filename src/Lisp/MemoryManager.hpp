#ifndef LISP_MEMORYMANAGER_HPP
#define LISP_MEMORYMANAGER_HPP

#include <vector>

#include "Configuration.hpp"

#include "GcPtr.hpp"
#include "Object.hpp"
#include "Value.hpp"

namespace Lisp
{
    class Context;

    /// A class responsible for object allocation and managing and garbage collection.
    class MemoryManager
    {
    public:
        MemoryManager();
        ~MemoryManager();

        MemoryManager(const MemoryManager& other) = delete;
        MemoryManager operator=(const MemoryManager& other) = delete;

        /// Allocate a managed object.
        // It looks so cool.
        template <typename T, typename... Args>
        GcPtr<T> AllocateObject(Args&&... args)
        {
            T* objPtr = new T(objects, std::forward<Args&&>(args)...);
            GcPtr<T> obj(objPtr);

            if (Configuration::GC::LOG)
            {
                LogObject("AllocateObject", obj);
            }

            objects = obj;

            return obj;
        }

        void CollectGarbageIfNeeded(Context& context, Value value, Value lexicalEnv);
        void CollectGarbage(Context& context, Value value, Value lexicalEnv);

        void PushProtect(GcPtr<Object> obj);
        void PopProtect();

        void LogBegin();
        void LogEnd();
        static void LogObject(const char* action, GcPtr<Object> obj);
        static void LogAction(const char* action);

    private:
        /// The head of the list of all allocated and living objects.
        /// WARINING: It is not a Lisp list. The next pointer is stored in every managed object.
        GcPtr<Object> objects;

        std::size_t objectsCount;
        std::size_t oldObjectsCount;
        std::size_t nextGC;

        bool ShouldDoGarbageCollection() const;
        void UpdateNextGC();

        std::vector<GcPtr<Object>> protectStack; // QUESTION: Why not Lisp list?

        void MarkRoots(Context& context, Value value, Value lexicalEnv);
        void Sweep();

        void DeleteObject(GcPtr<Object> obj);
    }; // class MemoryManager
}

#endif // LISP_MEMORYMANAGER_HPP
