#ifndef LISP_PRIMITIVES_CORE_HPP
#define LISP_PRIMITIVES_CORE_HPP

#include <stdexcept>

#include "../Value.hpp"
#include "../ObjectType.hpp"
#include "../Context.hpp"

#include "../Funcs/List.hpp"

namespace Lisp
{
    namespace Primitives
    {
        /// Check that value is a proper list and return the length of the value. If the check is failed, raise error.
        void EnsureProperList(Context& context, Value list);
        std::size_t EnsureProperListAndGetLength(Context& context, Value list);

        /// Check, that args is a proper list and have length `count`. If the check is failed, raise error.
        void EnsureArgumentsCount(Context& context, Value args, std::size_t count);

        void EnsureArgumentsCountAtLeast(Context& context, Value args, std::size_t count);

        /// Extract argument with specific type. If the argument's type is mismatched, then raise error.
        template <typename T>
        T ExtractValue(Context& context, Value value)
        {
            if (!value.Is<T>())
            {
                context.MakeAndRaiseWrongTypeError(value, typeid(T).name());
            }

            return value.As<T>();
        }

        /// Extract argument with specific type. If the argument's type is mismatched, then raise error.
        template <typename ObjType>
        GcPtr<ObjType> ExtractObject(Context& context, Value value)
        {
            if (!value.IsObject<ObjType>())
            {
                context.MakeAndRaiseWrongTypeError(value, ObjectTypeToCString(ObjType::GetStaticType()));
            }

            return value.AsObject<ObjType>();
        }

        template <typename T>
        T ExtractValueFromArgs(Context& context, Value args, std::size_t index)
        {
            return ExtractValue<T>(context, Funcs::Nth(args, index));
        }

        template <typename T>
        GcPtr<T> ExtractObjectFromArgs(Context& context, Value args, std::size_t index)
        {
            return ExtractObject<T>(context, Funcs::Nth(args, index));
        }

        /// Iterate over list and check that it is a proper list.
        /// The `fn` return value determines whether to continue iteration or not.
        void IterateOverLispList(Context& context, Value list, std::function<bool(Value)> fn);

        Value BoolToValue(Context& context, bool b);

        Value Eval(Context& context, Value args);

        Value SystemQuit(Context& context, Value args);

        Value SymbolIntern(Context& context, Value args);

        Value InternalGetStackTrace(Context& context, Value args);
    }
}

#endif // LISP_PRIMITIVES_CORE_HPP
