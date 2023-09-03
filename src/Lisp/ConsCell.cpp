#include "ConsCell.hpp"

#include "Funcs/List.hpp"

namespace Lisp
{
    ConsCell::ConsCell(GcPtr<Object> next, Value car, Value cdr)
        : Object(next, ObjectType::ConsCell), car(car), cdr(cdr)
    {
        
    }

    ObjectType ConsCell::GetStaticType()
    {
        return ObjectType::ConsCell;
    }

    std::ostream& PrintRest(std::ostream& out, const ConsCell& cell);

    std::ostream& ConsCell::Print(std::ostream& out) const
    {
        out << '(';
        return PrintRest(out, *this);
    }

    std::ostream& PrintRest(std::ostream& out, const ConsCell& cell)
    {
        Value car = cell.car;
        Value cdr = cell.cdr;

        out << car;

        if (cdr.IsNull())
        {
            out << ')';
        }
        else if (Funcs::Consp(cdr))
        {
            out << ' ';
            // There should be an iterative solution.
            PrintRest(out, *cdr.AsObject<ConsCell>());
        }
        else
        {
            out << " . " << cdr << ')';
        }

        return out;
    }

    void ConsCell::MarkChildren()
    {
        MarkValue(car);
        MarkValue(cdr);
    }
}
