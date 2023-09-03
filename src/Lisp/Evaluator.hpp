#ifndef LISP_EVALUATOR_HPP
#define LISP_EVALUATOR_HPP

#include "GcPtr.hpp"
#include "Object.hpp"
#include "Value.hpp"

#include "Symbol.hpp"
#include "ConsCell.hpp"

#include "SpecialFormEvaluator.hpp"

namespace Lisp
{
    class Context;
    
    class Evaluator
    {
    public:
        explicit Evaluator(Context& context);

        Value Eval(Value value, Value lexicalEnv);
        /// WARNING: This function won't check if `values` is a proper list.
        Value EvalExecute(Value values, Value lexicalEnv);

        SpecialFormEvaluator& GetSpecialFormEvaluator();

    private:
        Context& context;
        SpecialFormEvaluator specialFormEvaluator;

        Value EvalObject(GcPtr<Object> object, Value lexicalEnv);
        Value EvalSymbol(GcPtr<Symbol> symbol, Value lexicalEnv);
        Value EvalConsCell(GcPtr<ConsCell> cons, Value lexicalEnv);
        Value EvalConsCellRaw(GcPtr<ConsCell> cons, Value lexicalEnv);

        Value MacroExpand(Value macro, Value args, Value lexicalEnv);
    }; // class Evaluator
}

#endif // LISP_EVALUATOR_HPP
