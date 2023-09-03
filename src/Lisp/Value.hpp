#ifndef LISP_VALUE_HPP
#define LISP_VALUE_HPP

#include <iostream>
#include <variant>

#include "NilType.hpp"
#include "Primitive.hpp"

#include "GcPtr.hpp"
#include "Object.hpp"

namespace Lisp
{
    using Integer = long long;
    using Float = double;

    class Value
    {
    public:
        Value() = default;
        
        template <typename T>
        explicit Value(T value)
            : variant(value)
        {
            
        }

        template <typename T>
        explicit Value(GcPtr<T> ptr)
            : variant(ptr->template As<Object>())
        {
            
        }

        template <typename T>
        bool Is()
        {
            return std::holds_alternative<T>(variant);
        }

        bool IsObject()
        {
            return Is<GcPtr<Object>>();
        }

        template <typename T>
        bool IsObject()
        {
            return IsObject()
                && As<GcPtr<Object>>()->Is<T>();
        }

        template <typename T>
        T As()
        {
            return std::get<T>(variant);
        }

        GcPtr<Object> AsObject()
        {
            return std::get<GcPtr<Object>>(variant);
        }

        template <typename T>
        GcPtr<T> AsObject()
        {
            return AsObject()->As<T>();
        }

        template <class Visitor>
        void Visit(Visitor&& visitor) const
        {
            std::visit(visitor, variant);
        }

        bool operator==(const Value& other) const
        {
            return this->variant == other.variant;
        }

        bool IsNull() const
        {
            return std::holds_alternative<NilType>(variant);
        }

    private:
        using ValueVariant = std::variant<NilType, Integer, Float, Primitive, GcPtr<Object>>;

        static_assert(std::is_copy_constructible_v<ValueVariant>, "Lisp::ValueVariant should be able to be copied");

        ValueVariant variant;
    }; // class Value

    extern Value NIL;

    std::ostream& operator<<(std::ostream& out, const Lisp::Value& value);

    /// Mark value if it is an object.
    void MarkValue(Value value);
}

#endif // LISP_VALUE_HPP
