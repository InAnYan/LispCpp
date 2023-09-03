#include "Equality.hpp"

#include "Core.hpp"

#include "../Funcs/Equality.hpp"
#include "../Funcs/List.hpp"

namespace Lisp
{
    namespace Primitives
    {
        template <Funcs::EqualityFn Equality>
        Value GenericEquality(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Value a = Funcs::First(args);
            Value b = Funcs::Second(args);

            return BoolToValue(context, Equality(a, b));
        }

        Value Eq(Context& context, Value args)
        {
            return GenericEquality<Funcs::Eq>(context, args);
        }

        Value Eql(Context& context, Value args)
        {
            return GenericEquality<Funcs::Eql>(context, args);
        }
    }
}
