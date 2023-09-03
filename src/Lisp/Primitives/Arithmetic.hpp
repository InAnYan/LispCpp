#ifndef LISP_PRIMITIVES_ARITHMETIC_HPP
#define LISP_PRIMITIVES_ARITHMETIC_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value BinaryAdd(Context& context, Value args);
        Value BinarySubstract(Context& context, Value args);
        Value BinaryMultiply(Context& context, Value args);
        Value BinaryDivide(Context& context, Value args);

        Value BinaryGreater(Context& context, Value args);
        Value BinaryLess(Context& context, Value args);

        Value Negate(Context& context, Value args);

        Value Percent(Context& context, Value args);

        Value Round(Context& context, Value args);
        Value Floor(Context& context, Value args);
        Value Ceil(Context& context, Value args);
        Value Truncate(Context& context, Value args);

        Value Random(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_ARITHMETIC_HPP
