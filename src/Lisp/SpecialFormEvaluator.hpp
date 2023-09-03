#ifndef LISP_SPECIALFORMEVALUATOR_HPP
#define LISP_SPECIALFORMEVALUATOR_HPP

#include <unordered_map>
#include <utility>

#include "GcPtr.hpp"
#include "Value.hpp"
#include "SpecialForms.hpp"

#include "Symbol.hpp"
#include "ConsCell.hpp"

namespace Lisp
{
    class Context;

    class SpecialFormEvaluator
    {
    public:
        explicit SpecialFormEvaluator(Context& context);

        /// If the `cons` is a special form, evaluate it, store the result in `result` and return true. Otherwise, return false.
        bool EvalSpecialForm(GcPtr<ConsCell> cons, Value lexicalEnv, Value& result);

        void DefineSpecialForm(Util::StringWindow name, SpecialFormFn fn);
        
    private:
        Context& context;
        
        using InternalMap = std::unordered_map<GcPtr<Symbol>, SpecialFormFn>;
        InternalMap map;
    }; // class SpecialFormEvaluator
}

#endif // LISP_SPECIALFORMEVALUATOR_HPP
