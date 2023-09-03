#include "Equality.hpp"

namespace Lisp
{
    namespace Funcs
    {
        bool Eq(Value a, Value b)
        {
            // It will also check if numbers are equal.
            return a == b;
        }

        bool Eql(Value a, Value b)
        {
            return Eq(a, b);
        }
    }
}
