#ifndef LISP_OBJECTSINTERNER_HPP
#define LISP_OBJECTSINTERNER_HPP

#include <unordered_map>

#include "Util/StringWindow.hpp"

#include "GcPtr.hpp"
#include "MemoryManager.hpp"
#include "String.hpp"

namespace Lisp
{
    template <typename Key, typename StoringObject>
    class ObjectsInterner
    {
    public:
        explicit ObjectsInterner(MemoryManager& memory)
            : memory(memory)
        {
            
        }

        // WARNING: Be careful about the key lifetime.

        // NOTE: When we intern strings, the key of the entry is not the
        // argument `key`, but it is the string of the string object.
        GcPtr<Object> Intern(Key key)
        {
            typename InternalMap::iterator interned = map.find(key);

            if (interned != map.end())
            {
                return interned->second;
            }
            else
            {
                GcPtr<StoringObject> obj = memory.AllocateObject<StoringObject>(key);

                if constexpr (std::is_same_v<Key, Util::StringWindow> && std::is_same_v<StoringObject, String>)
                {
                    map[obj->MakeStringWindow()] = obj;
                }
                else
                {
                    map[key] = obj;
                }

                return obj;
            }
        }

        // WARNING: Keys are not marked.
        void MarkObjects()
        {
            for (auto& entry : map)
            {
                entry.second->Mark();
            }
        }

    private:
        MemoryManager& memory;

        using InternalMap = std::unordered_map<Key, GcPtr<StoringObject>>;
        InternalMap map;
    };
}

#endif // LISP_OBJECTSINTERNER_HPP
