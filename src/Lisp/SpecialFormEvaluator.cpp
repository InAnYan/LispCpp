#include "SpecialFormEvaluator.hpp"

#include "Context.hpp"

#include "Funcs/Predicates.hpp"

namespace Lisp
{
    SpecialFormEvaluator::SpecialFormEvaluator(Context& context)
        : context(context)
    {

    }

    bool SpecialFormEvaluator::EvalSpecialForm(GcPtr<ConsCell> cons, Value lexicalEnv, Value& result)
    {
        Value maybeSymbol = cons->car;

        if (!Funcs::Symbolp(maybeSymbol))
        {
            return false;
        }

        GcPtr<Symbol> symbol = maybeSymbol.AsObject<Symbol>();
        InternalMap::iterator entry = map.find(symbol);

        if (entry == map.end())
        {
            return false;
        }

        result = entry->second(context, cons->cdr, lexicalEnv);
        return true;
    }

    void SpecialFormEvaluator::DefineSpecialForm(Util::StringWindow name, SpecialFormFn fn)
    {
        GcPtr<String> string = context.GetStringsInterner().Intern(Util::StringWindow(name));
        GcPtr<Symbol> symbol = context.GetSymbolsInterner().Intern(string);
        map[symbol] = fn;
    }
}
