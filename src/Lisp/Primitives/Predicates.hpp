#ifndef LISP_PRIMITIVES_PREDICATES_HPP
#define LISP_PRIMITIVES_PREDICATES_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Nullp(Context& context, Value args);
        Value Consp(Context& context, Value args);
        Value Listp(Context& context, Value args);
        Value Callablep(Context& context, Value args);
        Value Lambdap(Context& context, Value args);
        Value Macrop(Context& context, Value args);
        Value Primitivep(Context& context, Value args);
        Value Integerp(Context& context, Value args);
        Value Floatp(Context& context, Value args);
        Value Numberp(Context& context, Value args);
        Value Stringp(Context& context, Value args);
        Value Symbolp(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_PREDICATES_HPP
