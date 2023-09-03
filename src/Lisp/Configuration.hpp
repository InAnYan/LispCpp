#ifndef LISP_CONFIGURATION_HPP
#define LISP_CONFIGURATION_HPP

namespace Lisp
{
    // Actually, whenever I have a namespace I also create a new folder.
    // But this is an excpetion, because `Configuration::DEBUG_MODE` looks
    // nicer than `DEBUG_MODE`.
    namespace Configuration
    {
        constexpr bool DEBUG_MODE = true;
        // At the begining, I wanted to make two functions:
        // `Context::Eval` and `Context::EvalWithStacktrace`,
        // but I haven't found a way to eliminate code duplication.
        // So I decided to write only one function and tinker its behaviour
        // via `Configuration`.

        namespace GC
        {
            /// Collect garbage every on `Context::Eval` call.
            // NOTE: Don't stress GC when logging it. Or just don't load prelude.
            constexpr bool STRESS = false;

            constexpr bool LOG = false;
        }

        namespace Reader
        {
            constexpr bool LOG = false;
        }

        namespace Eval
        {
            constexpr bool LOG = false;
            // NOTE: Only Lisp forms are tracked in stack trace.
            constexpr bool STACK_TRACE = true;
        }
    }
}

#endif // LISP_CONFIGURATION_HPP
