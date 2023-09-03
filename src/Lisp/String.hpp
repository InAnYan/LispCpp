#ifndef LISP_STRING_HPP
#define LISP_STRING_HPP

#include <string>

#include "Util/StringWindow.hpp"

#include "GcPtr.hpp"
#include "Object.hpp"

namespace Lisp
{
    /// Lisp string object.
    class String final : public Object
    {
    public:
        String(GcPtr<Object> next, std::string&& str);
        String(GcPtr<Object> next, const std::string& str);
        String(GcPtr<Object> next, Util::StringWindow window);
        
        const std::string& GetCppString() const;
        Util::StringWindow MakeStringWindow() const;

        virtual std::ostream& Print(std::ostream& out) const;

        static ObjectType GetStaticType();

    private:
        std::string str;

        virtual void MarkChildren();
    }; // class String
}

#endif // LISP_STRING_HPP
