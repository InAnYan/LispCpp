#ifndef LISP_SPECIALFORMS_HPP
#define LISP_SPECIALFORMS_HPP

#include "Value.hpp"

namespace Lisp
{
    class Context;

    using SpecialFormFn = Value(*)(Context& context, Value args, Value lexicalEnv);

    Value Quote(Context& context, Value args, Value lexicalEnv);
    Value If(Context& context, Value args, Value lexicalEnv);
    Value Execute(Context& context, Value args, Value lexicalEnv);
    Value Lambda(Context& context, Value args, Value lexicalEnv);
    Value Macro(Context& context, Value args, Value lexicalEnv);
    Value Define(Context& context, Value args, Value lexicalEnv);
    Value LetOne(Context& context, Value args, Value lexicalEnv);
    Value MacroExpand(Context& context, Value args, Value lexicalEnv);
    Value Cond(Context& context, Value args, Value lexicalEnv);
    Value Redefine(Context& context, Value args, Value lexicalEnv);
}

#endif // LISP_SPECIALFORMS_HPP
