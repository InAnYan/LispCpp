#ifndef LISP_NILTYPE_HPP
#define LISP_NILTYPE_HPP

#include <iostream>

namespace Lisp
{
    /// Dummy class. It is created to overload `operator<<` for `std::ostream`'s.
    class NilType
    {
    public:
        inline bool operator==([[maybe_unused]] const NilType& other) const
        {
            return true;
        }
    }; // class NilType

    std::ostream& operator<<(std::ostream& out, const NilType& nil);
}


#endif // LISP_NILTYPE_HPP
