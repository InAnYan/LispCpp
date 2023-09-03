#include "List.hpp"

#include "Core.hpp"

#include "../Funcs/List.hpp"
#include "../Funcs/Predicates.hpp"

namespace Lisp
{
    namespace Primitives
    {
        using AccessorFn = Value(*)(Value);

        template <AccessorFn Fn>
        Value GenericAccessor(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value list = Funcs::First(args);

            if (list.IsNull())
            {
                return NIL;
            }
            else if (Funcs::Consp(list))
            {
                return Fn(list);
            }
            else
            {
                context.MakeAndRaiseWrongTypeError(list, "List");
                return NIL;
            }
        }

        Value First(Context& context, Value args)
        {
            return GenericAccessor<Funcs::First>(context, args);
        }

        Value Rest(Context& context, Value args)
        {
            return GenericAccessor<Funcs::Rest>(context, args);
        }

        using SetFn = void(*)(Value cons, Value value);

        template <SetFn Fn>
        Value GenericSetter(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            GcPtr<ConsCell> cons = ExtractObjectFromArgs<ConsCell>(context, args, 0);
            Value value = Funcs::Second(args);
                
            Fn(Value(cons), value);

            return Value(cons);
        }

        Value SetCar(Context& context, Value args)
        {
            return GenericSetter<Funcs::SetCar>(context, args);
        }

        Value SetCdr(Context& context, Value args)
        {
            return GenericSetter<Funcs::SetCdr>(context, args);
        }

        Value Cons(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Value car = Funcs::First(args);
            Value cdr = Funcs::Second(args);

            return Funcs::Cons(context, car, cdr);
        }

        Value List(Context& context, Value args)
        {
            return args;
        }
    }
}
