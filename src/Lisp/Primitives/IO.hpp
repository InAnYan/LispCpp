#ifndef LISP_PRIMITIVES_IO_HPP
#define LISP_PRIMITIVES_IO_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Print(Context& context, Value args);
        Value PrintChar(Context& context, Value args);
        Value PrintPretty(Context& context, Value args);
        Value PrintNewLine(Context& context, Value args);

        Value Load(Context& context, Value args);

        Value ReadLine(Context& context, Value args);

        Value Read(Context& context, Value args);
        Value ReadAll(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_IO_HPP
