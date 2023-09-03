#ifndef LISP_PRIMITIVES_EQUALITY_HPP
#define LISP_PRIMITIVES_EQUALITY_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Eq(Context& context, Value args);
        Value Eql(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_EQUALITY_HPP
