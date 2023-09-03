#include "Value.hpp"

namespace Lisp
{
    Value NIL = Value(NilType());

    void MarkValue(Value value)
    {
        if (value.IsObject())
        {
            value.AsObject()->Mark();
        }
    }

    std::ostream& operator<<(std::ostream& out, const Value& value)
    {
        value.Visit([&out](auto arg)
        {
            out << arg;
        });

        return out;
    }
}

