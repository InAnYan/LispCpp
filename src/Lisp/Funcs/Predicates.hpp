#ifndef LISP_FUNCS_PREDICATES_HPP
#define LISP_FUNCS_PREDICATES_HPP

#include "../Value.hpp"

namespace Lisp
{
    namespace Funcs
    {
        using PredicateFn = bool (*)(Value);

        bool Consp(Value value);
        bool Listp(Value value);
        bool Lastp(Value value);
        bool Callablep(Value value);
        bool Lambdap(Value value);
        bool Macrop(Value value);
        bool Numberp(Value value);
        bool Stringp(Value value);
        bool Symbolp(Value value);
    }
}

#endif // LISP_FUNCS_PREDICATES_HPP
