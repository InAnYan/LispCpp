#include "Context.hpp"

#include <cassert>

#include "Funcs/Predicates.hpp"
#include "Funcs/List.hpp"
#include "Funcs/Functional.hpp"

#include "Exceptions/ErrorWhileHandlingError.hpp"
#include "Exceptions/AbortEvaluation.hpp"

namespace Lisp
{
    Context::Context(std::istream& input, std::ostream& output)
        : input(input), output(output), strings(memory), symbols(memory), evaluator(*this), mersenne(randomDevice()), isRaisingError(false)
    {
        
    }

    std::istream& Context::GetInputStream()
    {
        return input;
    }

    std::ostream& Context::GetOutputStream()
    {
        return output;
    }

    MemoryManager& Context::GetMemoryManager()
    {
        return memory;
    }

    Context::StringsInterner& Context::GetStringsInterner()
    {
        return strings;
    }

    Context::SymbolsInterner& Context::GetSymbolsInterner()
    {
        return symbols;
    }

    Evaluator& Context::GetEvaluator()
    {
        return evaluator;
    }

    Value Context::Eval(Value value, Value lexicalEnv)
    {
        return evaluator.Eval(value, lexicalEnv);
    }

    Value Context::EvalExecute(Value values, Value lexicalEnv)
    {
        return evaluator.EvalExecute(values, lexicalEnv);
    }

    Value Context::InternSymbol(Util::StringWindow string)
    {
        if (string == Util::StringWindow("nil"))
        {
            return NIL;
        }
        else
        {
            return Value(symbols.Intern(strings.Intern(string)));
        }
    }

    void Context::Define(Util::StringWindow name, Value value)
    {
        Value symbolValue = InternSymbol(name);

        GcPtr<Symbol> symbol = symbolValue.AsObject<Symbol>();

        assert(symbol->GetGlobalValueList().IsNull() && "when defining global variable the symbol should not be defined before");

        symbol->PushGlobalValue(*this, value);
    }

    void Context::DefinePrimitive(Util::StringWindow name, PrimitiveFn fn)
    {
        Define(name, Value(Primitive(fn)));
    }
    
    void Context::DefineSpecialForm(Util::StringWindow name, SpecialFormFn fn)
    {
        evaluator.GetSpecialFormEvaluator().DefineSpecialForm(name, fn);
    }

    Integer Context::GenerateRandomNumber()
    {
        return mersenne();
    }

    Value Context::GetStackTrace()
    {
        return stackTrace;
    }

    void Context::PushStackTrace(Value value)
    {
        Funcs::Push(*this, value, stackTrace);
    }

    void Context::PopStackTrace()
    {
        assert(Funcs::Consp(stackTrace));
        stackTrace = Funcs::Rest(stackTrace);
    }

    Value Context::MakeInternalError(const char* type, const char* fmt, std::initializer_list<Value> fmtLst, std::initializer_list<const char*> restarts)
    {
        Value restartsLispList = NIL;

        for (const char* cStr : restarts)
        {
            Funcs::Push(*this, InternSymbol(Util::StringWindow(cStr)), restartsLispList);
        }

        Value fmtLispList = Funcs::List(*this, fmtLst);

        Value fmtObj = AllocateObject<String>(fmt);
        Value typeObj = InternSymbol(Util::StringWindow(type));

        return Funcs::List(*this, std::initializer_list<Value>{ InternSymbol(Util::StringWindow("internal-error")), typeObj, fmtObj, fmtLispList, restartsLispList });
    }

    Value Context::RaiseInternalError(Value error)
    {
        if (isRaisingError)
        {
            throw Exceptions::ErrorWhileHandlingError();
        }

        isRaisingError = true;

        Value errorHandler = InternSymbol(Util::StringWindow("*internal-error-handler*")).AsObject<Symbol>()->GetGlobalValue(*this);

        Value restart = Funcs::Apply(*this, errorHandler, Funcs::Cons(*this, error, NIL), NIL);

        if (!Funcs::Symbolp(restart))
        {
            throw Exceptions::ErrorWhileHandlingError();
        }

        isRaisingError = false;

        return restart;
    }

    Value Context::MakeAndRaiseInternalError(const char* type, const char* fmt, std::initializer_list<Value> fmtLst, std::initializer_list<const char*> restarts)
    {
        return RaiseInternalError(MakeInternalError(type, fmt, fmtLst, restarts));
    }

    void Context::MakeAndRaiseWrongTypeError(Value value, const char* type)
    {
        MakeAndRaiseErrorOnlyAbort("wrong-type",
                                   "The value~%        ~a~%    is not of type~%        ~a",
                                   { value, InternSymbol(Util::StringWindow(type)) });
    }

    void Context::MakeAndRaiseErrorOnlyAbort(const char* type, const char* fmt, std::initializer_list<Value> fmtLst)
    {
        Value error = MakeInternalError(type, fmt, fmtLst, { "abort-evaluation" });
        RaiseInternalError(error);
        throw Exceptions::AbortEvaluation();
    }

    void Context::MakeAndRaiseErrorOnlyAbort(const char* type, const char* fmt)
    {
        return MakeAndRaiseErrorOnlyAbort(type, fmt, {});
    }
}
