#ifndef LISP_PRIMITIVE_HPP
#define LISP_PRIMITIVE_HPP

#include <iostream>

namespace Lisp
{
    class Value;
    class Context;
    using PrimitiveFn = Value(*)(Context& context, Value args);

    class Primitive
    {
    public:
        explicit Primitive(PrimitiveFn fn);
        
        PrimitiveFn GetFn() const;
        // I could write `operator()`, but the `Value` is not defined.

        bool operator==(const Primitive& other) const;

    private:
        PrimitiveFn fn;
    }; // class Primitive

    std::ostream& operator<<(std::ostream& out, const Primitive& primitive);
}

#endif // LISP_PRIMITIVE_HPP
