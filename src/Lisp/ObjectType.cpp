#include "ObjectType.hpp"

#include <sstream>

namespace Lisp
{
    const char* ObjectTypeToCString(ObjectType type)
    {
#define LISP_OBJECT_TYPE_CSTR(name) \
        case ObjectType::name: return #name; break;

        switch (type)
        {
            LISP_OBJECT_TYPE_LIST(LISP_OBJECT_TYPE_CSTR);
        default:
            return "Unknown";
        }
#undef LISP_OBJECT_TYPE_PRINT
    }
    
    std::ostream& operator<<(std::ostream& out, ObjectType type)
    {
        return out << ObjectTypeToCString(type);
    }
}
