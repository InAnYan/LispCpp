#include "LambdaFunction.hpp"

namespace Lisp
{
    LambdaFunction::LambdaFunction(GcPtr<Object> next, bool isMacro, Value params, Value body /* , Value closure */)
        : Object(next, ObjectType::LambdaFunction), isMacro(isMacro), params(params), body(body)
    {
        
    }

    bool LambdaFunction::IsMacro() const
    {
        return isMacro;
    }

    Value LambdaFunction::GetParams()
    {
        return params;
    }

    Value LambdaFunction::GetBody()
    {
        return body;
    }

    std::ostream& LambdaFunction::Print(std::ostream& out) const
    {
        if (isMacro)
        {
            return out << "<macro>";
        }
        else
        {
            return out << "<lambda>";
        }
    }

    ObjectType LambdaFunction::GetStaticType()
    {
        return ObjectType::LambdaFunction;
    }

    void LambdaFunction::MarkChildren()
    {
        MarkValue(params);
        MarkValue(body);
    }
}
