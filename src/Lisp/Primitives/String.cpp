#include "String.hpp"

#include "Core.hpp"

#include "../Funcs/List.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value SymbolString(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<Symbol> symbol = ExtractObjectFromArgs<Symbol>(context, args, 0);

            return Value(symbol->GetString());
        }

        Value StringLength(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);

            // QUESTION: Probably unsafe?
            return Value(static_cast<Integer>(string->GetCppString().size()));
        }

        Value StringBinaryAppend(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            GcPtr<String> firstObject = ExtractObjectFromArgs<String>(context, args, 0);
            GcPtr<String> secondObject = ExtractObjectFromArgs<String>(context, args, 1);

            const std::string& firstString = firstObject->GetCppString();
            const std::string& secondString = secondObject->GetCppString();

            std::string newString = firstString + secondString;

            // TODO: That's bad because the programm allocates new string twice, instead of taking
            // new string.
            return Value(context.GetStringsInterner().Intern(Util::StringWindow(newString)));
        }

        Value CharAt(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);
            Integer index = ExtractValueFromArgs<Integer>(context, args, 1);

            Integer size = string->GetCppString().size();

            if (index < 0 || index > size)
            {
                // It's possible to add `change-index` restart. But I'm too lazy. (And don't really like this error system.).
                context.MakeAndRaiseErrorOnlyAbort("wrong-string-index", "Wrong string index.");
                return NIL;
            }

            return Value(string->GetCppString()[index]);
        }

        Value Substring(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 3);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);
            Integer beginIndex = ExtractValueFromArgs<Integer>(context, args, 1);
            Integer endIndex = ExtractValueFromArgs<Integer>(context, args, 2);

            Integer size = string->GetCppString().size();

            if (beginIndex < 0 || endIndex < 0
                || beginIndex > size || endIndex > size)
            {
                context.MakeAndRaiseErrorOnlyAbort("wrong-string-index", "Wrong string index.");
                return NIL;
            }

            std::string newString = string->GetCppString().substr(beginIndex, endIndex - beginIndex);

            return Value(context.GetStringsInterner().Intern(Util::StringWindow(newString)));
        }

        Value StringEqual(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 2);

            GcPtr<String> firstString = ExtractObjectFromArgs<String>(context, args, 0);
            GcPtr<String> secondString = ExtractObjectFromArgs<String>(context, args, 1);

            return BoolToValue(context, firstString->GetCppString() == secondString->GetCppString());
        }

        Value CharCode(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);

            return Value(string->GetCppString()[0]);
        }

        Value CharCodeToString(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Integer num = ExtractValueFromArgs<Integer>(context, args, 0);

            return context.AllocateObject<String>(std::string(1, static_cast<char>(num)));
        }
    }
}
