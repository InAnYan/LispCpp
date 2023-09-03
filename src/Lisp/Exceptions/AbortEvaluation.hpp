#ifndef LISP_EXCEPTIONS_ABORTEVALUATION_HPP
#define LISP_EXCEPTIONS_ABORTEVALUATION_HPP

#include "SpecialRuntimeException.hpp"

namespace Lisp
{
    namespace Exceptions
    {
        class AbortEvaluation : public SpecialRuntimeException
        {
        public:
            AbortEvaluation();
            
            virtual const char* what() const throw();
        }; // class AbortEvaluation
        
        std::ostream& operator<<(std::ostream& out, const AbortEvaluation& e);
    }
}

#endif // LISP_EXCEPTIONS_ABORTEVALUATION_HPP
