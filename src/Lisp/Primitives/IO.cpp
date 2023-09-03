#include "IO.hpp"

#include <iostream>

#include "Core.hpp"

#include "../Funcs/List.hpp"
#include "../Funcs/IO.hpp"

namespace Lisp
{
    namespace Primitives
    {
        Value Print(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            context.GetOutputStream() << value;

            return value;
        }

        Value PrintChar(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Integer ch = ExtractValueFromArgs<Integer>(context, args, 0);

            context.GetOutputStream() << static_cast<char>(ch);

            return Value(ch);
        }
        
        Value PrintPretty(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            Value value = Funcs::First(args);

            if (Funcs::Stringp(value))
            {
                context.GetOutputStream() << value.AsObject<String>()->GetCppString();
            }
            else
            {
                context.GetOutputStream() << value;
            }

            return value;
        }

        Value PrintNewLine(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 0);

            context.GetOutputStream() << std::endl;

            return NIL;
        }

        Value Load(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> path = ExtractObjectFromArgs<String>(context, args, 0);

            return Funcs::Load(context, path->GetCppString());
        }

        Value ReadLine(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 0);

            std::string string;
            if (!std::getline(context.GetInputStream(), string))
            {
                context.MakeAndRaiseErrorOnlyAbort("io-error", "Some error occured while reading line form stdout.");
            }

            return context.AllocateObject<String>(string);
        }

        Value Read(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);

            return Funcs::Read(context, string);
        }

        Value ReadAll(Context& context, Value args)
        {
            EnsureArgumentsCount(context, args, 1);

            GcPtr<String> string = ExtractObjectFromArgs<String>(context, args, 0);

            return Funcs::ReadAll(context, string);
        }
    }
}
