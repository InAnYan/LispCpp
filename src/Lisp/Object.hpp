#ifndef LISP_OBJECT_HPP
#define LISP_OBJECT_HPP

#include <iostream>

#include "ObjectType.hpp"
#include "GcPtr.hpp"

namespace Lisp
{
    /// The root of all managed objects in the Interpreter.
    class Object
    {
    public:
        virtual ~Object() = default;

        ObjectType GetType() const;
        bool Is(ObjectType type) const;

        GcPtr<Object> GetNext();
        void SetNext(GcPtr<Object> newNext);

        void Mark();
        bool IsMarked() const;
        void Unmark();

        virtual std::ostream& Print(std::ostream& out) const = 0;

        template <typename T>
        static constexpr bool IsValidLispObject()
        {
            // The Lisp object is valid if it is inherited from Object and has a static method
            // ObjectType GetStatictype().
            static_assert(std::is_base_of<Object, T>::value && "the type should inherit from Lisp::Object");
            static_assert(std::is_same<decltype(T::GetStaticType()), ObjectType>::value,
                          "the type should implement a static method Lisp::ObjectType GetStaticType()");

            return true;
        }

        template <typename T>
        bool Is() const
        {
            static_assert(IsValidLispObject<T>() && "the type is not an Lisp::Object");
            return Is(T::GetStaticType());
        }

        // QUESTION: GCC, what's the problem?
        // template <>
        template <std::same_as<Object> T>
        GcPtr<Object> As()
        {
            return GcPtr<Object>(this);
        }

        // template <>
        template <std::same_as<Object> T>
        GcPtr<const Object> As() const
        {
            return GcPtr<const Object>(this);
        }
        
        template <typename T>
        GcPtr<T> As()
        {
            // This template is so cool. No need to create AsString, AsSymbol and so on.
            // And with GetStaticType we can even check for correctness.
            static_assert(IsValidLispObject<T>() && "the type is not an Lisp::Object");
            assert(GetType() == T::GetStaticType() && "bad cast for Lisp::Object");
            return GcPtr<T>(reinterpret_cast<T*>(this));
        }

        template <typename T>
        GcPtr<const T> As() const
        {
            static_assert(IsValidLispObject<T>() && "the type is not an Lisp::Object");
            assert(GetType() == T::GetStaticType() && "bad cast for Lisp::Object");
            return GcPtr<const T>(reinterpret_cast<const T*>(this));
        }

    protected:
        // You cannot instantiate a Object, only its derivatives.
        Object(GcPtr<Object> next, ObjectType type);

    private:
        bool isMarked;
        ObjectType type;
        GcPtr<Object> next;

        virtual void MarkChildren() = 0;
    }; // class Object

    std::ostream& operator<<(std::ostream& out, const Object& obj);
}

#endif // LISP_OBJECT_HPP
