#ifndef LISP_FUNCS_EQUALITY_HPP
#define LISP_FUNCS_EQUALITY_HPP

#include "../Value.hpp"

namespace Lisp
{
    namespace Funcs
    {
        using EqualityFn = bool (*)(Value a, Value b);

        bool Eq(Value a, Value b);
        bool Eql(Value a, Value b);
    }
}

#endif // LISP_FUNCS_EQUALITY_HPP
