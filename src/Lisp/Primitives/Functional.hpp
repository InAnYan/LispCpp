#ifndef LISP_PRIMITIVES_FUNCTIONAL_HPP
#define LISP_PRIMITIVES_FUNCTIONAL_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Apply(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_FUNCTIONAL_HPP
