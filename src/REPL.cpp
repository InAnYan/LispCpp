#include "REPL.hpp"

#include "Lisp/Lisp.hpp"

REPL::REPL(Lisp::Context& context)
    : context(context)
{
    
}

int REPL::Start()
{
    try
    {
        RunLoop();
        return 0;
    }
    catch (const Lisp::Exceptions::ShouldQuit& e)
    {
        return e.GetExitcode();
    }
}

void REPL::RunLoop()
{
    while (true)
    {
        context.GetOutputStream() << "repl> ";

        std::string line;
        std::getline(context.GetInputStream(), line);

        try
        {
            ExecuteLine(line);
        }
        catch (const Lisp::Exceptions::AbortEvaluation& e)
        {
            continue;
        }
    }
}

void REPL::ExecuteLine(const std::string& line)
{
    Lisp::Reader reader(context, Lisp::Util::StringWindow(line));

    Lisp::Value value = reader.ReadValue();
    Lisp::ProtectValue protect(context.GetMemoryManager(), value);

    Lisp::Value evaluated = context.Eval(value, Lisp::NIL);

    context.GetOutputStream() << evaluated << std::endl;
}
