#ifndef LISP_PRIMITIVES_STRING_HPP
#define LISP_PRIMITIVES_STRING_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value SymbolString(Context& context, Value args);
        Value StringLength(Context& context, Value args);
        Value StringBinaryAppend(Context& context, Value args);
        Value CharAt(Context& context, Value args);
        Value Substring(Context& context, Value args);
        Value StringEqual(Context& context, Value args);
        Value CharCode(Context& context, Value args);
        Value CharCodeToString(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_STRING_HPP
