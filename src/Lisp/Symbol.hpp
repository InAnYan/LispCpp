#ifndef LISP_SYMBOL_HPP
#define LISP_SYMBOL_HPP

#include "GcPtr.hpp"
#include "Object.hpp"
#include "Value.hpp"

#include "String.hpp"

namespace Lisp
{
    class Context;
    
    /// Lisp symbol object.
    class Symbol final : public Object
    {
    public:
        Symbol(GcPtr<Object> next, GcPtr<String> string);

        GcPtr<String> GetString();
        const GcPtr<String> GetString() const;
        Value GetGlobalValueList(); // It is only used for GC.

        Value GetGlobalValue(Context& context);
        void PushGlobalValue(Context& context, Value value);
        void PopGlobalValue(Context& context);

        virtual std::ostream& Print(std::ostream& out) const;

        static ObjectType GetStaticType();
        
    private:
        GcPtr<String> string;

        /// A stack of global values for the symbol.
        // QUESTION: Why it's implemented as a Lisp's list? Why not std::list or std::vector?
        Value globalValuesList;

        virtual void MarkChildren();

        Value ProcessRestart(Context& context, Value restart);
    }; // class Symbol
}

#endif // LISP_SYMBOL_HPP
