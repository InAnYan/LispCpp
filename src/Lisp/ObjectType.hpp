#ifndef LISP_OBJECTTYPE_HPP
#define LISP_OBJECTTYPE_HPP

#include <iostream>

/// A macro of object types that accepts a macro. `o(name)`.
#define LISP_OBJECT_TYPE_LIST(o) \
    o(String) \
    o(Symbol) \
    o(ConsCell) \
    o(Primitive) \
    o(LambdaFunction)

namespace Lisp
{
    enum class ObjectType
    {
#define LISP_OBJECT_TYPE_ENUM(name) \
        name,

        LISP_OBJECT_TYPE_LIST(LISP_OBJECT_TYPE_ENUM)

#undef LISP_OBJECT_TYPE_ENUM
    }; // enum class ObjectType

    std::ostream& operator<<(std::ostream& out, ObjectType type);

    const char* ObjectTypeToCString(ObjectType type);
}

#endif // LISP_OBJECTTYPE_HPP
