#ifndef LISP_LAMBDAFUNCTION_HPP
#define LISP_LAMBDAFUNCTION_HPP

#include "GcPtr.hpp"
#include "Object.hpp"
#include "Value.hpp"

namespace Lisp
{
    // NOTE: Yeah, there are some big problems with const correctnes.
    
    class LambdaFunction final : public Object
    {
    public:
        LambdaFunction(GcPtr<Object> next, bool isMacro, Value params, Value body /* , Value closure */);

        bool IsMacro() const;
        Value GetParams();
        Value GetBody();

        virtual std::ostream& Print(std::ostream& out) const;

        static ObjectType GetStaticType();
        
    private:
        bool isMacro;
        Value params;
        Value body;

        virtual void MarkChildren();
    }; // class LambdaFunction
}

#endif // LISP_LAMBDAFUNCTION_HPP
