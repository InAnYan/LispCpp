#include "Functional.hpp"

#include "Core.hpp"

#include "../Funcs/List.hpp"
#include "../Funcs/Predicates.hpp"
#include "../Funcs/Functional.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Apply(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Value func = Funcs::First(args);
            Value funcArgs = Funcs::Second(args);

            // It's safe to call `Funcs::Apply`, because it checks types for `func`.
            return Funcs::Apply(context, func, funcArgs, NIL);
        }
    }
}
