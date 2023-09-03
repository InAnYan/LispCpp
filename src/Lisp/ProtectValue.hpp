#ifndef LISP_PROTECTVALUE_HPP
#define LISP_PROTECTVALUE_HPP

#include "Value.hpp"
#include "MemoryManager.hpp"

namespace Lisp
{
    // QUESTION: This class looks so fancy, but what is its preformance impact?

    class ProtectValue
    {
    public:
        ProtectValue(MemoryManager& memory, Value value);
        ~ProtectValue();

        ProtectValue(const ProtectValue& other) = delete;
        ProtectValue& operator=(const ProtectValue& other) = delete;

    private:
        MemoryManager& memory;
        bool isObject;
    }; // class ProtectValue
}

#endif // LISP_PROTECTVALUE_HPP
