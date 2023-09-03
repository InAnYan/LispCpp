#include "NilType.hpp"

namespace Lisp
{
    std::ostream& operator<<(std::ostream& out, [[maybe_unused]] const NilType& nil)
    {
        return out << "nil";
    }
}
