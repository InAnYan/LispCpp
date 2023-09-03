#include "ShouldQuit.hpp"

namespace Lisp
{
    namespace Exceptions
    {
        ShouldQuit::ShouldQuit(int exitCode)
            : exitCode(exitCode)
        {
            
        }
        
        const char* ShouldQuit::what() const throw()
        {
            return "the program requested quit";
        }
        
        int ShouldQuit::GetExitcode() const
        {
            return exitCode;
        }
        
        std::ostream& operator<<(std::ostream& out, const ShouldQuit& e)
        {
            return out << e.what();
        }
    }
}
