#ifndef LISP_EXCEPTIONS_SPECIALRUNTIMEEXCEPTION_HPP
#define LISP_EXCEPTIONS_SPECIALRUNTIMEEXCEPTION_HPP

#include <exception>
#include <iostream>

namespace Lisp
{
    namespace Exceptions
    {
        class SpecialRuntimeException : public std::exception
        {
            // NOTE: Classes should inherit from this class.
        }; // class SpecialRuntimeException
    }
}

#endif // LISP_EXCEPTIONS_SPECIALRUNTIMEEXCEPTION_HPP
