#include "Primitive.hpp"

namespace Lisp
{
    Primitive::Primitive(PrimitiveFn fn)
        : fn(fn)
    {
        
    }
        
    PrimitiveFn Primitive::GetFn() const
    {
        return fn;
    }

    bool Primitive::operator==(const Primitive& other) const
    {
        return this->fn == other.fn;
    }

    std::ostream& operator<<(std::ostream& out, [[maybe_unused]] const Primitive& primitive)
    {
        return out << "<primitive>";
    }
}
