#ifndef LISP_CONTEXT_HPP
#define LISP_CONTEXT_HPP

#include <random>

#include "Util/StringWindow.hpp"

#include "Value.hpp"
#include "MemoryManager.hpp"
#include "ObjectsInterner.hpp"
#include "SpecialForms.hpp"
#include "Evaluator.hpp"

#include "String.hpp"
#include "Symbol.hpp"
#include "ConsCell.hpp"

namespace Lisp
{
    class Context
    {
    public:
        using StringsInterner = ObjectsInterner<Util::StringWindow, String>;
        using SymbolsInterner = ObjectsInterner<GcPtr<String>, Symbol>;

        Context(std::istream& input, std::ostream& output);

        std::istream& GetInputStream();
        std::ostream& GetOutputStream();

        MemoryManager& GetMemoryManager();
        StringsInterner& GetStringsInterner();
        // WARNING: Symbols interner does not intern `NIL` symbol.
        SymbolsInterner& GetSymbolsInterner();
        Evaluator& GetEvaluator();

        /// A shorthand for evaluation.
        /// Another way to do the same is: `Context::GetEvaluator().Eval(value, lexicalEnv)`.
        Value Eval(Value value, Value lexicalEnv);
        /// A shorthand for evaluation.
        /// Another way to do the same is: `Context::GetEvaluator().EvalExecute(values, lexicalEnv)`.
        Value EvalExecute(Value values, Value lexicalEnv);

        /// A shorthand for allocating objects.
        /// Another way to do the same is: `Context::GetMemoryManager().AllocateObject<T>(Args&&...)`.
        template <typename T, typename... Args>
        Value AllocateObject(Args&&... args)
        {
            return Value(memory.AllocateObject<T>(std::forward<Args&&>(args)...));
        }

        /// A shorthand for interning symbols. Checks for `NIL`.
        Value InternSymbol(Util::StringWindow string);

        /// Creates global variable. The symbol should not be defined before.
        void Define(Util::StringWindow name, Value value);
        /// Creates global variable that stores a primitive. The symbol should not be defined before.
        /// Another way to do the same is: `Context::Define(name, Value(fn))`.
        void DefinePrimitive(Util::StringWindow name, PrimitiveFn fn);

        /// The symbol should not be defined before.
        /// Another way to do the same is: `Context::GetEvaluator().GetSpecialFormEvaluator().DefineSpecialForm(name, fn)`.
        void DefineSpecialForm(Util::StringWindow name, SpecialFormFn fn);

        Integer GenerateRandomNumber();

        Value GetStackTrace();
        void PushStackTrace(Value value);
        void PopStackTrace();

        Value MakeInternalError(const char* type, const char* fmt, std::initializer_list<Value> fmtLst, std::initializer_list<const char*> restarts);

        /// Raises internal error. Returns user-selected restart.
        Value RaiseInternalError(Value error);

        Value MakeAndRaiseInternalError(const char* type, const char* fmt, std::initializer_list<Value> fmtLst, std::initializer_list<const char*> restarts);

        /// The only restart is `abort-evaluation`.
        void MakeAndRaiseWrongTypeError(Value value, const char* type);

        void MakeAndRaiseErrorOnlyAbort(const char* type, const char* fmt, std::initializer_list<Value> fmtLst);
        void MakeAndRaiseErrorOnlyAbort(const char* type, const char* fmt);

    private:
        std::istream& input;
        std::ostream& output;

        MemoryManager memory;

        StringsInterner strings;
        SymbolsInterner symbols;

        Evaluator evaluator;

        std::random_device randomDevice;
        std::mt19937 mersenne;

        Value stackTrace;

        bool isRaisingError;
    }; // class Context
}

#endif // LISP_CONTEXT_HPP
