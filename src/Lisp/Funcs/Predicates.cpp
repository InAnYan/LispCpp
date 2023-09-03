#include "Predicates.hpp"

#include "List.hpp"

#include "../String.hpp"
#include "../Symbol.hpp"
#include "../ConsCell.hpp"
#include "../Primitive.hpp"
#include "../LambdaFunction.hpp"

namespace Lisp
{
    namespace Funcs
    {
        template <typename T>
        bool IsValueIsSpecificObjectType(Value value)
        {
            return value.IsObject<T>();
        }

        template <typename First, typename Second, typename... Rest>
        bool IsValueIsSpecificObjectType(Value value)
        {
            return value.IsObject<First>() ||
                IsValueIsSpecificObjectType<Second, Rest...>(value);
        }

        bool Consp(Value value)
        {
            return IsValueIsSpecificObjectType<ConsCell>(value);
        }

        bool Listp(Value value)
        {
            return value.IsNull() || Consp(value);
        }

        bool Lastp(Value value)
        {
            return Consp(value) && Rest(value).IsNull();
        }

        bool Callablep(Value value)
        {
            return value.Is<Primitive>() || IsValueIsSpecificObjectType<LambdaFunction>(value);
        }

        template <bool shouldBeMacro>
        bool GenericLambdaFunctionp(Value value)
        {
            return IsValueIsSpecificObjectType<LambdaFunction>(value)
                && (value.AsObject<LambdaFunction>()->IsMacro() == shouldBeMacro);
        }

        bool Lambdap(Value value)
        {
            return GenericLambdaFunctionp<false>(value);
        }

        bool Macrop(Value value)
        {
            return GenericLambdaFunctionp<true>(value);
        }
            
        bool Numberp(Value value)
        {
            return value.Is<Integer>() || value.Is<Float>();
        }

        bool Stringp(Value value)
        {
            return IsValueIsSpecificObjectType<String>(value);
        }

        bool Symbolp(Value value)
        {
            return IsValueIsSpecificObjectType<Symbol>(value);
        }
    }
}
