#include "AbortEvaluation.hpp"

namespace Lisp
{
    namespace Exceptions
    {
        AbortEvaluation::AbortEvaluation()
        {
            
        }
        
        const char* AbortEvaluation::what() const throw()
        {
            return "evaluation abort was requested";
        }
        
        std::ostream& operator<<(std::ostream& out, const AbortEvaluation& e)
        {
            return out << e.what();
        }
    }
}
