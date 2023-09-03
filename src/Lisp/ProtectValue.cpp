#include "ProtectValue.hpp"

namespace Lisp
{
    ProtectValue::ProtectValue(MemoryManager& memory, Value value)
        : memory(memory), isObject(value.IsObject())
    {
        if (isObject)
        {
            memory.PushProtect(value.AsObject());
        }
    }

    ProtectValue::~ProtectValue()
    {
        if (isObject)
        {
            memory.PopProtect();
        }
    }
}
