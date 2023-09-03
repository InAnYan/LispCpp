#include "Evaluator.hpp"

#include <cassert>

#include "Context.hpp"
#include "ProtectValue.hpp"

#include "Funcs/List.hpp"
#include "Funcs/Predicates.hpp"
#include "Funcs/Functional.hpp"

#include "LambdaFunction.hpp"

#include "Primitives/Core.hpp"

#include "Exceptions/SpecialRuntimeException.hpp"

namespace Lisp
{
    Evaluator::Evaluator(Context& context)
        : context(context), specialFormEvaluator(context)
    {
        
    }

    Value Evaluator::Eval(Value value, Value lexicalEnv)
    {
        if (Configuration::Eval::LOG)
        {
            std::cout << "--- Eval: Evaluating: " << value << std::endl;
        }

        assert(Funcs::Listp(lexicalEnv) && "lexical environment should be a list");

        context.GetMemoryManager().CollectGarbageIfNeeded(context, value, lexicalEnv);

        Value result;

        if (value.IsObject())
        {
            result = EvalObject(value.AsObject(), lexicalEnv);
        }
        else
        {
            result = value;
        }

        if (Configuration::Eval::LOG)
        {
            std::cout << "--- Eval: Result: " << result << std::endl;
        }

        return result;
    }

    Value Evaluator::EvalExecute(Value values, Value lexicalEnv)
    {
        Value result = NIL;

        Funcs::IterateOverLispList(values, [this, &result, lexicalEnv](Value value)
        {
            result = Eval(value, lexicalEnv);
        });

        return result;
    }

    Value Evaluator::EvalObject(GcPtr<Object> object, Value lexicalEnv)
    {
        if (object->Is<Symbol>())
        {
            return EvalSymbol(object->As<Symbol>(), lexicalEnv);
        }
        else if (object->Is<ConsCell>())
        {
            return EvalConsCell(object->As<ConsCell>(), lexicalEnv);
        }
        else
        {
            return Value(object);
        }
    }

    Value Evaluator::EvalSymbol(GcPtr<Symbol> symbol, Value lexicalEnv)
    {
        Value entry = Funcs::Assoc<Funcs::Eq>(Value(symbol), lexicalEnv);

        if (!entry.IsNull())
        {
            return Funcs::Rest(entry);
        }
        else
        {
            return symbol->GetGlobalValue(context);
        }
    }

    Value Evaluator::EvalConsCell(GcPtr<ConsCell> cons, Value lexicalEnv)
    {
        if (Configuration::Eval::STACK_TRACE)
        {
            try
            {
                context.PushStackTrace(Value(cons));
                Value result = EvalConsCellRaw(cons, lexicalEnv);
                context.PopStackTrace();
                return result;
            }
            catch (const Exceptions::SpecialRuntimeException& e)
            {
                context.PopStackTrace();
                throw;
            }
        }
        else
        {
            return EvalConsCellRaw(cons, lexicalEnv);
        }
    }

    Value Evaluator::EvalConsCellRaw(GcPtr<ConsCell> cons, Value lexicalEnv)
    {
        Value specialFormResult;
        if (specialFormEvaluator.EvalSpecialForm(cons, lexicalEnv, specialFormResult))
        {
            return specialFormResult;
        }

        Value func = Eval(cons->car, lexicalEnv);

        if (!Funcs::Callablep(func))
        {
            context.MakeAndRaiseErrorOnlyAbort("non-callable-value", "Attempt to call non-callable value.");
            return NIL;
        }

        if (func.IsObject<LambdaFunction>())
        {
            GcPtr<LambdaFunction> lambda = func.AsObject<LambdaFunction>();
            if (lambda->IsMacro())
            {
                return MacroExpand(func, cons->cdr, lexicalEnv);
            }
        }

        Value evalList = Funcs::Cons(context, func, NIL);
        ProtectValue protect(context.GetMemoryManager(), evalList);
        
        Value evalIt = evalList;

        Primitives::IterateOverLispList(context, cons->cdr,
                                        [this, lexicalEnv, &evalIt](Value arg)
                                        {
                                            Value argEvaluated = Eval(arg, lexicalEnv);
                                            Value argCell = Funcs::Cons(context, argEvaluated, NIL);

                                            Funcs::SetCdr(evalIt, argCell);
                                            evalIt = argCell;
                                            
                                            return true;
                                        });

        Value args = Funcs::Rest(evalList);

        Value result = Funcs::Apply(context, func, args, lexicalEnv);
        return result;
    }

    Value Evaluator::MacroExpand(Value macro, Value args, Value lexicalEnv)
    {
        ProtectValue protectMacro(context.GetMemoryManager(), macro);
        Value resultExpr = Funcs::Apply(context, macro, args, lexicalEnv);
        ProtectValue protectResult(context.GetMemoryManager(), resultExpr);

        return context.Eval(resultExpr, lexicalEnv);
    }

    SpecialFormEvaluator& Evaluator::GetSpecialFormEvaluator()
    {
        return specialFormEvaluator;
    }
}
