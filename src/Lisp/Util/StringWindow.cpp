#include "StringWindow.hpp"

#include <cstring>
#include <stdexcept>
#include <charconv>
#include <system_error>

namespace Lisp
{
    namespace Util
    {
        StringWindow::StringWindow(std::string::const_iterator beginIt, std::string::const_iterator endIt)
            : begin(beginIt), end(endIt)
        {
            
        }

        StringWindow::StringWindow(const std::string& str)
            : begin(str.begin()), end(str.end())
        {
            
        }

        StringWindow::StringWindow(const char* literal)
            : begin(literal), end(literal + std::strlen(literal))
        {
            
        }

        std::string StringWindow::ToCppString() const
        {
            return std::string(begin, end);
        }

        bool StringWindow::operator==(const StringWindow& other) const
        {
            // Now you see, that begin and end should be equal or in distance (end > begin).
            std::string::const_iterator otherIt = other.begin;

            // Look closely at the for clauses.
            for (std::string::const_iterator thisIt = this->begin; thisIt != this->end; ++thisIt, ++otherIt)
            {
                if (otherIt == other.end || *thisIt != *otherIt)
                {
                    return false;
                }
            }

            return otherIt == other.end;
        }
    }
}
