#ifndef LISP_EXCEPTIONS_ERRORWHILEHANDLINGERROR_HPP
#define LISP_EXCEPTIONS_ERRORWHILEHANDLINGERROR_HPP

#include <exception>
#include <iostream>

namespace Lisp
{
    namespace Exceptions
    {
        class ErrorWhileHandlingError : public std::exception
        {
        public:
            ErrorWhileHandlingError();
            
            virtual const char* what() const throw();
        }; // class ErrorWhileHandlingError
        
        std::ostream& operator<<(std::ostream& out, const ErrorWhileHandlingError& e);
    }
}

#endif // LISP_EXCEPTIONS_ERRORWHILEHANDLINGERROR_HPP
