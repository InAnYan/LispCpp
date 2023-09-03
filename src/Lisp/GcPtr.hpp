#ifndef LISP_GCPTR_HPP
#define LISP_GCPTR_HPP

#include <iostream>
#include <functional>
#include <cassert>

namespace Lisp
{
    /// A template type that signs that an object is managed by GC (aka `MemoryManager`).
    template <typename T>
    class GcPtr
    {
    public:
        GcPtr()
            : ptr(nullptr)
        {
            
        }

        explicit GcPtr(T* obj)
            : ptr(obj)
        {
            
        }

        GcPtr(const GcPtr<T>& other) = default;

        ~GcPtr() = default;

        bool IsNullptr() const
        {
            return ptr == nullptr;
        }

        T* const operator->()
        {
            assert(!IsNullptr() && "dereferencing nullptr GcPtr");
            return ptr;
        }

        const T* const operator->() const
        {
            assert(!IsNullptr() && "dereferencing nullptr GcPtr");
            return ptr;
        }

        T& operator*()
        {
            assert(!IsNullptr() && "dereferencing nullptr GcPtr");
            return *ptr;
        }

        const T& operator*() const
        {
            assert(!IsNullptr() && "dereferencing nullptr GcPtr");
            return *ptr;
        }

        template <typename U>
        operator GcPtr<U>()
        {
            static_assert(std::is_base_of<T, U>::value || std::is_base_of<U, T>::value,
                          "invalid cast for Lisp::GcPtr, types should be in inheritance");
            // return GcPtr<U>(reinterpret_cast<U*>(this));
            // I leave that for the history. It was so hilarious. But fairly easy to find the problem.
            return GcPtr<U>(reinterpret_cast<U*>(ptr));
        }

        template <typename Return, typename... Args>
        Return operator()(Args&&... args) const
        {
            return (*ptr)(std::forward<Args&&>(args)...);
        }

        std::size_t Hash() const
        {
            return std::hash<T*>{}(ptr);
        }

        bool operator==(const GcPtr<T>& other) const
        {
            return this->ptr == other.ptr;
        }

        bool operator!=(const GcPtr<T>& other) const
        {
            return !(*this == other);
        }

        operator bool() const
        {
            return !IsNullptr();
        }

        void Delete() const
        {
            delete ptr;
        }

        std::ostream& PrintPointerValue(std::ostream& out) const
        {
            return out << ptr;
        }

    private:
        T* ptr;
    }; // class GcPtr

    template <typename T>
    inline std::ostream& operator<<(std::ostream& out, const GcPtr<T>& gcPtr)
    {
        assert(!gcPtr.IsNullptr() && "dereferencing nullptr GcPtr");
        return out << *gcPtr;
    }
}

template <typename T>
struct std::hash<Lisp::GcPtr<T>>
{
    std::size_t operator()(const Lisp::GcPtr<T>& gcPtr) const
    {
        return gcPtr.Hash();
    }
};

#endif // LISP_GCPTR_HPP
