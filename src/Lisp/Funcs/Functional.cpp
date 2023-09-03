#include "Functional.hpp"

#include <cassert>

#include "Predicates.hpp"
#include "List.hpp"

#include "../Primitive.hpp"
#include "../LambdaFunction.hpp"
#include "../ProtectValue.hpp"

#include "../Primitives/Core.hpp"

namespace Lisp
{
    namespace Funcs
    {
        Value ApplyLambda(Context& context, GcPtr<LambdaFunction> func, Value args);

        Value Apply(Context& context, Value func, Value args, Value lexicalEnv)
        {
            ProtectValue protectLexicalEnv(context.GetMemoryManager(), lexicalEnv);

            if (func.Is<Primitive>())
            {
                return func.As<Primitive>().GetFn()(context, args);
            }
            else if (func.IsObject<LambdaFunction>())
            {

                return ApplyLambda(context, func.AsObject<LambdaFunction>(), args);
            }
            else
            {
                context.MakeAndRaiseErrorOnlyAbort("non-callable-value", "Attempt to call non-callable value.");
                return NIL;
            }
        }

        Value MakeLambdaLexicalEnv(Context& context, Value params, Value args);

        Value ApplyLambda(Context& context, GcPtr<LambdaFunction> func, Value args)
        {
            Value lexicalEnv = MakeLambdaLexicalEnv(context, func->GetParams(), args);
            return context.EvalExecute(func->GetBody(), lexicalEnv);
        }

        Value MakeLambdaLexicalEnv(Context& context, Value params, Value args)
        {
            Value lexicalEnv = NIL;

            Value paramsIt = params;
            Value argsIt = args;

            bool optionalMode = false;

            while (!argsIt.IsNull())
            {
                if (paramsIt.IsNull())
                {
                    context.MakeAndRaiseErrorOnlyAbort("too-much-arguments", "Too much arguments passed to function.");
                    return NIL;
                }
                    
                Primitives::ExtractObject<ConsCell>(context, paramsIt);
                Primitives::ExtractObject<ConsCell>(context, argsIt);

                Value param = Funcs::First(paramsIt);
                if (optionalMode && Funcs::Consp(param))
                {
                    param = Funcs::First(param);
                }

                Value arg = Funcs::First(argsIt);

                if (Funcs::Eq(param, context.InternSymbol(Util::StringWindow("&rest"))))
                {
                    paramsIt = Funcs::Rest(paramsIt);
                    Primitives::ExtractObject<ConsCell>(context, paramsIt);

                    param = Funcs::First(paramsIt);
                    arg = argsIt;

                    Value entry = Funcs::Cons(context, param, arg);
                    Funcs::Push(context, entry, lexicalEnv);

                    paramsIt = Funcs::Rest(paramsIt);
                    break;
                }
                else if (Funcs::Eq(param, context.InternSymbol(Util::StringWindow("&optional"))))
                {
                    paramsIt = Funcs::Rest(paramsIt);
                    optionalMode = true;
                    continue;
                }

                Value entry = Funcs::Cons(context, param, arg);
                Funcs::Push(context, entry, lexicalEnv);

                paramsIt = Funcs::Rest(paramsIt);
                argsIt = Funcs::Rest(argsIt);
            }

            while (!paramsIt.IsNull())
            {
                Primitives::ExtractObject<ConsCell>(context, paramsIt);
                Value param = Funcs::First(paramsIt);
                Value arg = NIL;
                if (optionalMode && Funcs::Consp(param))
                {
                    Primitives::ExtractObject<ConsCell>(context, Funcs::Rest(param));
                    arg = Funcs::Second(param);
                    param = Funcs::First(param);
                }

                if (Funcs::Eq(param, context.InternSymbol(Util::StringWindow("&rest"))))
                {
                    paramsIt = Funcs::Rest(paramsIt);
                    Primitives::ExtractObject<ConsCell>(context, paramsIt);

                    param = Funcs::First(paramsIt);

                    Value entry = Funcs::Cons(context, param, NIL);
                    Funcs::Push(context, entry, lexicalEnv);

                    break;
                }
                else if (Funcs::Eq(param, context.InternSymbol(Util::StringWindow("&optional"))))
                {
                    paramsIt = Funcs::Rest(paramsIt);
                    optionalMode = true;
                    continue;
                }
                else if (optionalMode)
                {
                    Value entry = Funcs::Cons(context, param, arg);
                    Funcs::Push(context, entry, lexicalEnv);

                    paramsIt = Funcs::Rest(paramsIt);
                }
                else
                {
                    context.MakeAndRaiseErrorOnlyAbort("not-enough-arguments", "Not enough arguments passed to function.");
                    return NIL;
                }
            }

            return lexicalEnv;
        }
    }
}
