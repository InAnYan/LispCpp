#ifndef LISP_FUNCS_FUNCTIONAL_HPP
#define LISP_FUNCS_FUNCTIONAL_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Funcs
    {
        /// Will check if `func` is callable. Will check if `args` is proper list if the `func` is a lambda.
        /// The parameter `lexicalEnv` is unused, but protected from GC.
        Value Apply(Context& context, Value func, Value args, Value lexicalEnv);
    }
}

#endif // LISP_FUNCS_FUNCTIONAL_HPP
