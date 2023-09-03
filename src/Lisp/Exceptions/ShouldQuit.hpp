#ifndef LISP_EXCEPTIONS_SHOULDQUIT_HPP
#define LISP_EXCEPTIONS_SHOULDQUIT_HPP

#include "SpecialRuntimeException.hpp"

namespace Lisp
{
    namespace Exceptions
    {
        class ShouldQuit : public SpecialRuntimeException
        {
        public:
            explicit ShouldQuit(int exitCode);
            
            int GetExitcode() const;
            
            virtual const char* what() const throw();
            
        private:
            int exitCode;
        }; // class ShouldQuit
        
        std::ostream& operator<<(std::ostream& out, const ShouldQuit& e);
    }
}

#endif // LISP_EXCEPTIONS_SHOULDQUIT_HPP
