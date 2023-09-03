#include "SpecialForms.hpp"

#include "Funcs/List.hpp"
#include "Funcs/Predicates.hpp"
#include "Funcs/Functional.hpp"

#include "Primitives/Core.hpp"

#include "Exceptions/AbortEvaluation.hpp"

#include "LambdaFunction.hpp"

namespace Lisp
{
    Value Quote(Context& context, Value args, [[maybe_unused]] Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 1);

        return Funcs::First(args);
    }

    Value If(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 3);

        Value condition = Funcs::First(args);
        Value thenClause = Funcs::Second(args);
        Value elseClause = Funcs::Third(args);

        if (!context.Eval(condition, lexicalEnv).IsNull())
        {
            return context.Eval(thenClause, lexicalEnv);
        }
        else
        {
            return context.Eval(elseClause, lexicalEnv);
        }
    }
    
    Value Execute(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureProperList(context, args);

        return context.EvalExecute(args, lexicalEnv);
    }

    template <bool isMacro>
    Value GenericLambdaFunction(Context& context, Value args, [[maybe_unused]] Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCountAtLeast(context, args, 1);

        Value lambdaParams = Funcs::First(args);
        Value lambdaBody = Funcs::Rest(args);

        // NOTE: The correctnes of `lambdaParams` is not checked.

        // HISTORY NOTE:
        // Originally, I wrote this (the class name was `Lambda`):
        // return context.AllocateObject<Lambda>(lambdaArgs, lambdaBody);
        // But then I got absolutely unidentified error from compiler. Even changed clang to GCC.
        // The reason for this error was because `Lambda` is a function (this function),
        // and also `Lambda` was an object. So, I renamed `Lambda` object to `LambdaFunction`.

        return context.AllocateObject<LambdaFunction>(isMacro, lambdaParams, lambdaBody);
    }

    Value Lambda(Context& context, Value args, Value lexicalEnv)
    {
        return GenericLambdaFunction<false>(context, args, lexicalEnv);
    }

    Value Macro(Context& context, Value args, Value lexicalEnv)
    {
        return GenericLambdaFunction<true>(context, args, lexicalEnv);
    }

    Value DefineProcessRestart(Context& context, Value restart, GcPtr<Symbol> symbol, Value newValue);

    Value Define(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 2);

        GcPtr<Symbol> name = Primitives::ExtractObjectFromArgs<Symbol>(context, args, 0);
        Value value = context.Eval(Funcs::Second(args), lexicalEnv);

        if (!name->GetGlobalValueList().IsNull())
        {
            // abort-evaluation
            // redefine
            Value restart = context.MakeAndRaiseInternalError("unbound-symbol",
                                                              "Symbol ~a is unbound.", { Value(name) },
                                                              { "abort-evaluation", "redefine" });
            return DefineProcessRestart(context, restart, name, value);
        }

        name->PushGlobalValue(context, value);

        return Funcs::First(args);
    }

    Value DefineProcessRestart(Context& context, Value restart, GcPtr<Symbol> symbol, Value newValue)
    {
        if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
        {
            throw Exceptions::AbortEvaluation();
        }
        else
        {
            symbol->PopGlobalValue(context);
            symbol->PushGlobalValue(context, newValue);
            return Value(symbol);
        }
    }

    Value LetOne(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 3);

        GcPtr<Symbol> var = Primitives::ExtractObjectFromArgs<Symbol>(context, args, 0);
        Value value = context.Eval(Funcs::Second(args), lexicalEnv);
        // We don't have to protect the evaluated `value`, because it will be stored
        // either in lexical environment or in global values list of the symbol.

        Value body = Funcs::Third(args);

        if (!var->GetGlobalValueList().IsNull())
        {
            var->PushGlobalValue(context, value);
            Value result = context.EvalExecute(body, lexicalEnv);
            var->PopGlobalValue(context);

            return result;
        }
        else
        {
            Value entry = Funcs::Cons(context, Value(var), value);
            Value newLexicalEnv = Funcs::Cons(context, entry, lexicalEnv);
            return context.Eval(body, newLexicalEnv);
        }
    }

    Value MacroExpand(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 1);

        GcPtr<ConsCell> expr = Primitives::ExtractObjectFromArgs<ConsCell>(context, args, 0);

        Value maybeMacro = context.Eval(expr->car, lexicalEnv);

        if (!Funcs::Macrop(maybeMacro))
        {
            context.MakeAndRaiseWrongTypeError(maybeMacro, "Macro");
        }

        return Funcs::Apply(context, maybeMacro, expr->cdr, lexicalEnv);
    }

    Value Cond(Context& context, Value args, Value lexicalEnv)
    {
        Value result = NIL;
        
        Primitives::IterateOverLispList(context, args, [&result, &context, lexicalEnv](Value value)
        {
            GcPtr<ConsCell> consClause = Primitives::ExtractObject<ConsCell>(context, value);

            Value conditionResult = context.Eval(consClause->car, lexicalEnv);

            if (!conditionResult.IsNull())
            {
                result = context.EvalExecute(consClause->cdr, lexicalEnv);
                return false;
            }

            return true;
        });

        return result;
    }

    Value RedefineProcessRestartNotBound(Context& context, GcPtr<Symbol> symbol, Value restart, Value newValue);
    Value RedefineProcessRestartNotOneBound(Context& context, GcPtr<Symbol> symbol, Value restart, Value newValue);

    Value Redefine(Context& context, Value args, Value lexicalEnv)
    {
        Primitives::EnsureArgumentsCount(context, args, 2);

        GcPtr<Symbol> name = Primitives::ExtractObjectFromArgs<Symbol>(context, args, 0);
        Value value = context.Eval(Funcs::Second(args), lexicalEnv);

        // Probably I should check the symbol before evaluating new value.

        if (name->GetGlobalValueList().IsNull())
        {
            Value restart = context.MakeAndRaiseInternalError("unbound-symbol",
                                                              "Symbol ~a is unbound. Could not redefine it.", { Value(name) },
                                                              { "abort-evaluation", "just-define" });
            return RedefineProcessRestartNotBound(context, name, restart, value);
        }
        else if (!Funcs::Lastp(name->GetGlobalValueList()))
        {
            Value restart = context.MakeAndRaiseInternalError("many-bound-symbol",
                                                              "Symbol ~a is bounds to many values. Expected single binding.", { Value(name) },
                                                              { "abort-evaluation", "change=last-value" });
            return RedefineProcessRestartNotBound(context, name, restart, value);
        }

        name->PopGlobalValue(context);
        name->PushGlobalValue(context, value);

        return Value(name);
    }

    Value RedefineProcessRestartNotBound(Context& context, GcPtr<Symbol> symbol, Value restart, Value newValue)
    {
        if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
        {
            throw Exceptions::AbortEvaluation();
        }
        else
        {
            symbol->PushGlobalValue(context, newValue);
            return Value(symbol);
        }
    }

    Value RedefineProcessRestartNotOneBound(Context& context, GcPtr<Symbol> symbol, Value restart, Value newValue)
    {
        if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
        {
            throw Exceptions::AbortEvaluation();
        }
        else
        {
            symbol->PopGlobalValue(context);
            symbol->PushGlobalValue(context, newValue);
            return Value(symbol);
        }
    }
}
