#include "Arithmetic.hpp"

#include <functional>
#include <iostream>
#include <cmath>

#include "Core.hpp"

#include "../Funcs/List.hpp"
#include "../Funcs/Predicates.hpp"

namespace Lisp
{
    namespace Primitives
    {
        template <template <typename T> typename BinOp>
        Value BinaryOperation(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Value a = Funcs::First(args);
            Value b = Funcs::Second(args);

            if (a.Is<Float>())
            {
                if (b.Is<Float>())
                {
                    return Value(BinOp<Float>{}(a.As<Float>(), b.As<Float>()));
                }
                else if (b.Is<Integer>())
                {
                    return Value(BinOp<Float>{}(a.As<Float>(), static_cast<Float>(b.As<Integer>())));
                }
                else
                {
                    context.MakeAndRaiseWrongTypeError(b, "Number");
                    return NIL;
                }
            }
            else if (a.Is<Integer>())
            {
                if (b.Is<Float>())
                {
                    return Value(BinOp<Float>{}(static_cast<Float>(a.As<Integer>()), b.As<Float>()));
                }
                else if (b.Is<Integer>())
                {
                    if (std::is_same_v<BinOp<Integer>, std::multiplies<Integer>>
                        || std::is_same_v<BinOp<Integer>, std::divides<Integer>>)
                    {
                        return Value(BinOp<Float>{}(a.As<Integer>(), b.As<Integer>()));
                    }
                    else
                    {
                        return Value(BinOp<Integer>{}(a.As<Integer>(), b.As<Integer>()));
                    }
                }
                else
                {
                    context.MakeAndRaiseWrongTypeError(b, "Number");
                    return NIL;
                }
            }
            else
            {
                context.MakeAndRaiseWrongTypeError(a, "Number");
                return NIL;
            }
        }

        Value BinaryAdd(Context& context, Value args)
        {
            return BinaryOperation<std::plus>(context, args);
        }

        Value BinarySubstract(Context& context, Value args)
        {
            return BinaryOperation<std::minus>(context, args);
        }

        Value BinaryMultiply(Context& context, Value args)
        {
            return BinaryOperation<std::multiplies>(context, args);
        }

        class ZeroDivisionError : std::exception
        {
            
        };

        template <typename T>
        struct safe_divides
        {
            T operator()(T a, T b) const
            {
                if (b == T())
                {
                    throw ZeroDivisionError();
                }
                else
                {
                    return a / b;
                }
            }
        };

        Value BinaryDivide(Context& context, Value args)
        {
            try
            {
                return BinaryOperation<safe_divides>(context, args);
            }
            catch (const ZeroDivisionError& e)
            {
                context.MakeAndRaiseErrorOnlyAbort("zero-division", "A number was divided by zero");
                return NIL;
            }
        }

        template <template <typename T> typename BinOp>
        Value GenericComparison(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Value a = Funcs::First(args);
            Value b = Funcs::Second(args);

            bool result;

            if (a.Is<Integer>())
            {
                if (b.Is<Integer>())
                {
                    result = BinOp{}(a.As<Integer>(), b.As<Integer>());
                }
                else if (b.Is<Float>())
                {
                    result = BinOp{}(a.As<Integer>(), b.As<Float>());
                }
                else
                {
                    context.MakeAndRaiseWrongTypeError(b, "Number");
                    return NIL;
                }
            }
            else if (a.Is<Float>())
            {
                if (b.Is<Integer>())
                {
                    result = BinOp{}(a.As<Float>(), b.As<Integer>());
                }
                else if (b.Is<Float>())
                {
                    result = BinOp{}(a.As<Float>(), b.As<Float>());
                }
                else
                {
                    context.MakeAndRaiseWrongTypeError(b, "Number");
                    return NIL;
                }
            }
            else
            {
                context.MakeAndRaiseWrongTypeError(a, "Number");
                return NIL;
            }

            return BoolToValue(context, result);
        }

        Value BinaryGreater(Context& context, Value args)
        {
            return GenericComparison<std::greater>(context, args);
        }

        Value BinaryLess(Context& context, Value args)
        {
            return GenericComparison<std::less>(context, args);
        }

        Value Negate(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            if (value.Is<Integer>())
            {
                return Value(-value.As<Integer>());
            }
            else if (value.Is<Float>())
            {
                return Value(value.As<Float>());
            }
            else
            {
                context.MakeAndRaiseWrongTypeError(value, "Number");
                return NIL;
            }
        }

        Value Percent(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            Integer a = ExtractValueFromArgs<Integer>(context, args, 0);
            Integer b = ExtractValueFromArgs<Integer>(context, args, 1);

            // TODO: Possible error checking?
            return Value(a % b);
        }

        using FloatOp = Float(*)(Float);

        template <FloatOp Fn>
        Value GenericFloatOperation(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Float num = ExtractValueFromArgs<Float>(context, args, 0);

            return Value(static_cast<Float>(Fn(num)));
        }

        Value Round(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Float num = ExtractValueFromArgs<Float>(context, args, 0);

            return Value(static_cast<Integer>(std::llround(num)));
        }

        Value Floor(Context& context, Value args)
        {
            return GenericFloatOperation<std::floor>(context, args);
        }

        Value Ceil(Context& context, Value args)
        {
            return GenericFloatOperation<std::ceil>(context, args);
        }
        
        Value Truncate(Context& context, Value args)
        {
            return GenericFloatOperation<std::trunc>(context, args);
        }

        Value Random(Context& context, Value args)
        {
            std::size_t argsCount = EnsureProperListAndGetLength(context, args);

            if (argsCount == 1)
            {
                Integer limit = ExtractValueFromArgs<Integer>(context, args, 0);

                return Value(context.GenerateRandomNumber() % limit);
            }
            else if (argsCount == 0)
            {
                return Value(context.GenerateRandomNumber());
            }
            else
            {
                context.MakeAndRaiseErrorOnlyAbort("too-much-arguments", "random was called with too much arguments");
                return NIL;
            }
        }
    }
}
