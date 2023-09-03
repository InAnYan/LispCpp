#include "Predicates.hpp"

#include "Core.hpp"

#include "../Funcs/List.hpp"
#include "../Funcs/Predicates.hpp"

namespace Lisp
{
    namespace Primitives
    {
        template <Funcs::PredicateFn Predicate>
        Value GenericPredicate(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            return BoolToValue(context, Predicate(value));
        }

        Value Nullp(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            return BoolToValue(context, value.IsNull());
        }

        Value Consp(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Consp>(context, args);
        }

        Value Listp(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Listp>(context, args);
        }

        Value Callablep(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Callablep>(context, args);
        }

        Value Lambdap(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Lambdap>(context, args);
        }

        Value Macrop(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Macrop>(context, args);
        }

        template <typename T>
        Value GenericValueLevelPredicate(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            return BoolToValue(context, value.Is<T>());
        }

        Value Primitivep(Context& context, Value args)
        {
            return GenericValueLevelPredicate<Primitive>(context, args);
        }

        Value Integerp(Context& context, Value args)
        {
            return GenericValueLevelPredicate<Integer>(context, args);
        }

        Value Floatp(Context& context, Value args)
        {
            return GenericValueLevelPredicate<Float>(context, args);
        }

        Value Numberp(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Numberp>(context, args);
        }

        Value Stringp(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Stringp>(context, args);
        }
        
        Value Symbolp(Context& context, Value args)
        {
            return GenericPredicate<Funcs::Symbolp>(context, args);
        }
    }
}
