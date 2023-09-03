#ifndef LISP_FUNCS_IO_HPP
#define LISP_FUNCS_IO_HPP

#include "../Value.hpp"
#include "../Context.hpp"

namespace Lisp
{
    namespace Funcs
    {
        Value Load(Context& context, const std::string& path);

        Value Read(Context& context, Util::StringWindow input);
        Value ReadAll(Context& context, Util::StringWindow input);
        Value Read(Context& context, GcPtr<String> input);
        Value ReadAll(Context& context, GcPtr<String> input);
    }
}

#endif // LISP_FUNCS_IO_HPP
