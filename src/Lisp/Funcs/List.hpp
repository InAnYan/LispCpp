#ifndef LISP_FUNCS_LIST_HPP
#define LISP_FUNCS_LIST_HPP

#include <numeric>
#include <cassert>

#include "../Value.hpp"
#include "../Context.hpp"

#include "Equality.hpp"
#include "Predicates.hpp"

namespace Lisp
{
    namespace Funcs
    {
        /// Get the car from cons cell. Does not check for NIL.
        Value First(Value value);
        Value Second(Value value);
        Value Third(Value value);
        /// Get the cdr from cons cell. Does not check for NIL.
        Value Rest(Value value);

        void SetCar(Value cons, Value value);
        void SetCdr(Value cons, Value value);

        Value Cons(Context& context, Value car, Value cdr);

        template <typename Container>
        Value List(Context& context, const Container& values)
        {
            return std::accumulate(std::rbegin(values), std::rend(values), NIL,
                                   [&context](Value list, Value item)
            {
                return Cons(context, item, list);
            });
        }

        void Push(Context& context, Value item, Value& list);

        Value NReverse(Value list);

        Value Nth(Value list, std::size_t index);

        template <EqualityFn EqFn>
        Value Assoc(Value item, Value alist)
        {
            for (Value it = alist; !it.IsNull(); it = Rest(it))
            {
                Value entry = First(it);
                if (EqFn(First(entry), item))
                {
                    return entry;
                }
            }

            return NIL;
        }

        template <typename UnaryFunction>
        void IterateOverLispList(Value list, UnaryFunction fn)
        {
            assert(Listp(list));

            for (Value it = list; !it.IsNull(); it = Rest(it))
            {
                assert(Consp(it));

                Value value = Funcs::First(it);
                fn(value);
            }
        }
    }
}

#endif // LISP_FUNCS_LIST_HPP
