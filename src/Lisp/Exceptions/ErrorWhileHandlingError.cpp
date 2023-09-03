#include "ErrorWhileHandlingError.hpp"

namespace Lisp
{
    namespace Exceptions
    {
        ErrorWhileHandlingError::ErrorWhileHandlingError()
        {
            
        }
        
        const char* ErrorWhileHandlingError::what() const throw()
        {
            return "an internal error occured while handling an internal error";
        }
        
        std::ostream& operator<<(std::ostream& out, const ErrorWhileHandlingError& e)
        {
            return out << e.what();
        }
    }
}
