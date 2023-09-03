#include "Symbol.hpp"

#include "Funcs/List.hpp"
#include "Funcs/IO.hpp"

#include "Exceptions/AbortEvaluation.hpp"

namespace Lisp
{
    Symbol::Symbol(GcPtr<Object> next, GcPtr<String> string)
        : Object(next, ObjectType::Symbol), string(string), globalValuesList(NIL)
    {

    }
    
    GcPtr<String> Symbol::GetString()
    {
        return string;
    }

    const GcPtr<String> Symbol::GetString() const
    {
        return string;
    }

    Value Symbol::GetGlobalValueList()
    {
        return globalValuesList;
    }

    Value Symbol::GetGlobalValue(Context& context)
    {
        if (globalValuesList.IsNull())
        {
            Value restart = context.MakeAndRaiseInternalError("unbound-symbol",
                                                              "Symbol ~a is unbound.",
                                                              { Value(GcPtr<Object>(this)) },
                                                              { "abort-evaluation", "enter-value", "use-nil" });
            return ProcessRestart(context, restart);
        }
        else
        {
            return Funcs::First(globalValuesList);
        }
    }

    Value Symbol::ProcessRestart(Context& context, Value restart)
    {
        if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
        {
            throw Exceptions::AbortEvaluation();
        }
        else if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("use-nil"))))
        {
            return NIL;
        }
        else // Well, probably we should check for `"enter-value"`, but it is the last restart and we assuming that the error handler is good.
        {
            context.GetOutputStream() << "enter-value> ";

            std::string line;
            std::getline(context.GetInputStream(), line);

            Value value = Funcs::Read(context, Util::StringWindow(line));

            return context.Eval(value, NIL);
        }
    }

    void Symbol::PushGlobalValue(Context& context, Value value)
    {
        Funcs::Push(context, value, globalValuesList);
    }

    void PopGlobalValueProcessRestart(Context& context, Value restart);
    
    void Symbol::PopGlobalValue(Context& context)
    {
        if (globalValuesList.IsNull())
        {
            Value restart = context.MakeAndRaiseInternalError("unbound-symbol",
                                                              "Symbol ~a is unbound.",
                                                              { Value(GcPtr<Symbol>(this)) },
                                                              { "abort-evaluation", "do-nothing" });
            PopGlobalValueProcessRestart(context, restart);
        }
        else
        {
            globalValuesList = Funcs::Rest(globalValuesList);
        }
    }

    void PopGlobalValueProcessRestart(Context& context, Value restart)
    {
        if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
        {
            throw Exceptions::AbortEvaluation();
        }
        else
        {
            return;
        }
    }

    std::ostream& Symbol::Print(std::ostream& out) const
    {
        return out << string->GetCppString();
    }

    ObjectType Symbol::GetStaticType()
    {
        return ObjectType::Symbol;
    }

    void Symbol::MarkChildren()
    {
        string->Mark();
        MarkValue(globalValuesList);
    }
}
