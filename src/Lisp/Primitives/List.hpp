#ifndef LISP_PRIMITIVES_LIST_HPP
#define LISP_PRIMITIVES_LIST_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value First(Context& context, Value args);
        Value Rest(Context& context, Value args);

        Value SetCar(Context& context, Value args);
        Value SetCdr(Context& context, Value args);

        Value Cons(Context& context, Value args);
        Value List(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_LIST_HPP
