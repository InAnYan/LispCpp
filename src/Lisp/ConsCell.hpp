#ifndef LISP_CONSCELL_HPP
#define LISP_CONSCELL_HPP

#include "Value.hpp"

namespace Lisp
{
    class ConsCell final : public Object
    {
    public:
        ConsCell(GcPtr<Object> next, Value car, Value cdr);
        
        // Hope you are not scared of public fields.
        // Well, actually I'm scared a bit, because I'm used
        // to see `obj->GetField()` but not `obj->field`.

        Value car;
        Value cdr;

        virtual std::ostream& Print(std::ostream& out) const;

        static ObjectType GetStaticType();

    private:
        virtual void MarkChildren();
    }; // class ConsCell
}

#endif // LISP_CONSCELL_HPP
