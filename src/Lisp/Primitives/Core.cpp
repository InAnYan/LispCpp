#include "Core.hpp"

#include "../Reader.hpp"

#include "../Exceptions/ShouldQuit.hpp"

#include "../Funcs/Predicates.hpp"
#include "../Funcs/List.hpp"

namespace Lisp
{
    namespace Primitives
    {
        void EnsureProperList(Context& context, Value list)
        {
            IterateOverLispList(context, list, [](Value){ return true; });
        }

        std::size_t EnsureProperListAndGetLength(Context& context, Value list)
        {
            std::size_t result = 0;

            IterateOverLispList(context, list, [&result](Value)
            {
                result++;
                return true;
            });

            return result;
        }

        void EnsureArgumentsCount(Context& context, Value args, std::size_t count)
        {
            std::size_t argsRealCount = EnsureProperListAndGetLength(context, args);

            if (argsRealCount != count)
            {
                context.MakeAndRaiseErrorOnlyAbort("wrong-arguments-count",
                                                   "Passed wrong arguments count: ~a.",
                                                   { Value(static_cast<Integer>(argsRealCount)) });
            }
        }

        void EnsureArgumentsCountAtLeast(Context& context, Value args, std::size_t count)
        {
            std::size_t argsRealCount = EnsureProperListAndGetLength(context, args);

            if (argsRealCount < count)
            {
                context.MakeAndRaiseErrorOnlyAbort("wrong-arguments-count",
                                                   "Passed wrong arguments count: ~a.",
                                                   { Value(static_cast<Integer>(argsRealCount)) });
            }
        }

        void IterateOverLispList(Context& context, Value list, std::function<bool(Value)> fn)
        {
            if (list.IsNull())
            {
                return;
            }
            else if (Funcs::Consp(list))
            {
                for (Value it = list; !it.IsNull(); it = Funcs::Rest(it))
                {
                    GcPtr<ConsCell> cons = ExtractObject<ConsCell>(context, it);
                    if (!fn(cons->car))
                    {
                        break;
                    }
                }
            }
            else
            {
                context.MakeAndRaiseWrongTypeError(list, "List");
            }
        }

        Value BoolToValue(Context& context, bool b)
        {
            if (b)
            {
                return context.InternSymbol(Util::StringWindow("t"));
            }
            else
            {
                return NIL;
            }
        }

        Value Eval(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            return context.Eval(value, NIL);
        }

        Value SystemQuit(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Integer exitCode = ExtractValueFromArgs<Integer>(context, args, 0);

            throw Exceptions::ShouldQuit(exitCode);
        }

        Value SymbolIntern(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);

            return context.InternSymbol(Util::StringWindow(string->GetCppString()));
        }

        Value InternalGetStackTrace(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 0);

            return context.GetStackTrace();
        }
    }
    }
