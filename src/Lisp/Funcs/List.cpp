#include "List.hpp"

#include <cassert>

#include "Predicates.hpp"

namespace Lisp
{
    namespace Funcs
    {
        Value First(Value value)
        {
            assert(Consp(value));
            return value.AsObject<ConsCell>()->car;
        }

        Value Second(Value value)
        {
            return First(Rest(value));
        }

        Value Third(Value value)
        {
            return First(Rest(Rest(value)));
        }

        Value Rest(Value value)
        {
            assert(Consp(value));
            return value.AsObject<ConsCell>()->cdr;
        }

        void SetCar(Value cons, Value value)
        {
            assert(Consp(cons));
            cons.AsObject<ConsCell>()->car = value;
        }

        void SetCdr(Value cons, Value value)
        {
            assert(Consp(cons));
            cons.AsObject<ConsCell>()->cdr = value;
        }

        Value Cons(Context& context, Value car, Value cdr)
        {
            return context.AllocateObject<ConsCell>(car, cdr);
        }

        void Push(Context& context, Value item, Value& list)
        {
            list = Cons(context, item, list);
        }
        
        Value NReverse(Value list)
        {
            assert(Listp(list));

            Value current = list;
            Value previous = NIL;
            Value next = NIL;

            while (!current.IsNull())
            {
                next = Rest(current);
                SetCdr(current, previous);
                previous = current;
                current = next;
            }

            return previous;
        }

        Value Nth(Value args, std::size_t index)
        {
            for (std::size_t i = 0; i < index; i++)
            {
                args = Rest(args);
            }

            return First(args);
        }
    }
}
