#include "IO.hpp"

#include <fstream>
#include <sstream>

#include "../Reader.hpp"
#include "../ProtectValue.hpp"

#include "../Exceptions/ShouldQuit.hpp"
#include "../Exceptions/AbortEvaluation.hpp"

#include "../Funcs/List.hpp"

namespace Lisp
{
    namespace Funcs
    {
        Value LoadProcessRestart(Context& context, const std::string& path, Value restart);

        Value Load(Context& context, const std::string& path)
        {
            std::ifstream file(path);
            if (!file)
            {
                Value restart = context.MakeAndRaiseInternalError("open-file-error",
                                                                  "Unable to open file '~a'.",
                                                                  // Yeah, making a new string, not interning it.
                                                                  { context.AllocateObject<String>(path) },
                                                                  { "abort-evaluation", "retry", "change-path"});
                return LoadProcessRestart(context, path, restart);
            }

            std::stringstream ss;
            ss << file.rdbuf();

            std::string source = ss.str();

            Reader reader(context, Util::StringWindow(source));

            Value program = reader.ReadValues();
            ProtectValue protect(context.GetMemoryManager(), program);

            try
            {
                return context.EvalExecute(program, NIL);
            }
            catch (Exceptions::ShouldQuit& e)
            {
                return Value(e.GetExitcode());
            }
            catch (Exceptions::AbortEvaluation& e)
            {
                return NIL;
            }

            return Value(0);
        }

        Value LoadProcessRestart(Context& context, const std::string& path, Value restart)
        {
            if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("abort-evaluation"))))
            {
                throw Exceptions::AbortEvaluation();
            }
            else if (Funcs::Eq(restart, context.InternSymbol(Util::StringWindow("retry"))))
            {
                return Load(context, path);
            }
            else
            {
                while (true)
                {
                    context.GetOutputStream() << "new-path> ";
                    std::string line;
                    std::getline(context.GetInputStream(), line);

                    Value value = Funcs::Read(context, Util::StringWindow(line));
                    if (!value.IsObject<String>())
                    {
                        context.GetOutputStream() << "Not a string." << std::endl;
                        continue;
                    }
                    else
                    {
                        return Load(context, value.AsObject<String>()->GetCppString());
                    }
                }
            }
        }

        Value Read(Context& context, Util::StringWindow input)
        {
            Reader reader(context, input);
            return reader.ReadValue();
        }

        Value ReadAll(Context& context, Util::StringWindow input)
        {
            Reader reader(context, input);
            return reader.ReadValues();
        }

        Value Read(Context& context, GcPtr<String> input)
        {
            return Read(context, input->MakeStringWindow());
        }

        Value ReadAll(Context& context, GcPtr<String> input)
        {
            return ReadAll(context, input->MakeStringWindow());
        }
    }
}
