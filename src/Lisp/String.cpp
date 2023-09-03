#include "String.hpp"

#include <iomanip>

namespace Lisp
{
    String::String(GcPtr<Object> next, std::string&& str)
        : Object(next, ObjectType::String), str(str)
    {
        
    }

    String::String(GcPtr<Object> next, const std::string& str)
        : Object(next, ObjectType::String), str(str)
    {
        
    }

    String::String(GcPtr<Object> next, Util::StringWindow window)
        : String(next, window.ToCppString())
    {
        
    }
    
    std::ostream& String::Print(std::ostream& out) const
    {
        return out << std::quoted(str);
    }

    const std::string& String::GetCppString() const
    {
        return str;
    }

    Util::StringWindow String::MakeStringWindow() const
    {
        return Util::StringWindow(str);
    }

    ObjectType String::GetStaticType()
    {
        return ObjectType::String;
    }

    void String::MarkChildren()
    {
        
    }
}
