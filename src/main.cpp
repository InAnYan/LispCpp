#include <iostream>
#include <fstream>
#include <sstream>

#include "Lisp/Lisp.hpp"

#include "REPL.hpp"

void InitGlobalEnvironment(Lisp::Context& context, int argc, char* argv[]);
void AddSpecialForms(Lisp::Context& context);
void LoadPrelude(Lisp::Context& context);

int RunFile(Lisp::Context& context, const char* path);
int RunRepl(Lisp::Context& context);

int main(int argc, char* argv[])
{
    Lisp::Context context(std::cin, std::cout);
    InitGlobalEnvironment(context, argc, argv);
    AddSpecialForms(context);
    LoadPrelude(context);

    if (argc > 1)
    {
        return RunFile(context, argv[1]);
    }
    else
    {
        return RunRepl(context);
    }
}

int RunFile(Lisp::Context& context, const char* path)
{
    Lisp::Value result = Lisp::Funcs::Load(context, path);
        
    if (result.Is<Lisp::Integer>())
    {
        return result.As<Lisp::Integer>();
    }
    else
    {
        return 0;
    }
}

int RunRepl(Lisp::Context& context)
{
    REPL repl(context);
    return repl.Start();
}

void AddArgs(Lisp::Context& context, int argc, char* argv[]);
void DefinePrimitive(Lisp::Context& context, const char* name, Lisp::PrimitiveFn fn);

void InitGlobalEnvironment(Lisp::Context& context, int argc, char* argv[])
{
    AddArgs(context, argc, argv);

    DefinePrimitive(context, "eval",                     Lisp::Primitives::Eval);
    DefinePrimitive(context, "system-quit",              Lisp::Primitives::SystemQuit);
    DefinePrimitive(context, "symbol-intern",            Lisp::Primitives::SymbolIntern);
    DefinePrimitive(context, "internal-get-stack-trace", Lisp::Primitives::InternalGetStackTrace);
    DefinePrimitive(context, "read",                     Lisp::Primitives::Read);
    DefinePrimitive(context, "read-all",                 Lisp::Primitives::ReadAll);

    DefinePrimitive(context, "binary-add",               Lisp::Primitives::BinaryAdd);
    DefinePrimitive(context, "binary-substract",         Lisp::Primitives::BinarySubstract);
    DefinePrimitive(context, "binary-multiply",          Lisp::Primitives::BinaryMultiply);
    DefinePrimitive(context, "binary-divide",            Lisp::Primitives::BinaryDivide);
    DefinePrimitive(context, "binary-greater",           Lisp::Primitives::BinaryGreater);
    DefinePrimitive(context, "binary-less",              Lisp::Primitives::BinaryLess);
    DefinePrimitive(context, "negate",                   Lisp::Primitives::Negate);
    DefinePrimitive(context, "%",                        Lisp::Primitives::Percent);
    DefinePrimitive(context, "floor",                    Lisp::Primitives::Floor);
    DefinePrimitive(context, "round",                    Lisp::Primitives::Round);
    DefinePrimitive(context, "ceil",                     Lisp::Primitives::Ceil);
    DefinePrimitive(context, "truncate",                 Lisp::Primitives::Truncate);
    DefinePrimitive(context, "random",                   Lisp::Primitives::Random);

    DefinePrimitive(context, "null-p",                   Lisp::Primitives::Nullp);
    DefinePrimitive(context, "cons-p",                   Lisp::Primitives::Consp);
    DefinePrimitive(context, "list-p",                   Lisp::Primitives::Listp);
    DefinePrimitive(context, "callable-p",               Lisp::Primitives::Callablep);
    DefinePrimitive(context, "lambda-p",                 Lisp::Primitives::Lambdap);
    DefinePrimitive(context, "macro-p",                  Lisp::Primitives::Macrop);
    DefinePrimitive(context, "primitive-p",              Lisp::Primitives::Primitivep);
    DefinePrimitive(context, "integer-p",                Lisp::Primitives::Integerp);
    DefinePrimitive(context, "float-p",                  Lisp::Primitives::Floatp);
    DefinePrimitive(context, "number-p",                 Lisp::Primitives::Numberp);
    DefinePrimitive(context, "string-p",                 Lisp::Primitives::Stringp);
    DefinePrimitive(context, "symbol-p",                 Lisp::Primitives::Symbolp);

    DefinePrimitive(context, "eq",                       Lisp::Primitives::Eq);
    DefinePrimitive(context, "eql",                      Lisp::Primitives::Eql);

    DefinePrimitive(context, "apply",                    Lisp::Primitives::Apply);

    DefinePrimitive(context, "first",                    Lisp::Primitives::First);
    DefinePrimitive(context, "rest",                     Lisp::Primitives::Rest);
    DefinePrimitive(context, "set-car",                  Lisp::Primitives::SetCar);
    DefinePrimitive(context, "set-cdr",                  Lisp::Primitives::SetCdr);
    DefinePrimitive(context, "cons",                     Lisp::Primitives::Cons);
    DefinePrimitive(context, "list",                     Lisp::Primitives::List);

    DefinePrimitive(context, "symbol-string",            Lisp::Primitives::SymbolString);
    DefinePrimitive(context, "string-length",            Lisp::Primitives::StringLength);
    DefinePrimitive(context, "string-binary-append",     Lisp::Primitives::StringBinaryAppend);
    DefinePrimitive(context, "string-equal",             Lisp::Primitives::StringEqual);
    DefinePrimitive(context, "char-at",                  Lisp::Primitives::CharAt);
    DefinePrimitive(context, "string-substring",         Lisp::Primitives::Substring);
    DefinePrimitive(context, "char-code",                Lisp::Primitives::CharCode);

    DefinePrimitive(context, "print",                    Lisp::Primitives::Print);
    DefinePrimitive(context, "print-char",               Lisp::Primitives::PrintChar);
    DefinePrimitive(context, "print-pretty",             Lisp::Primitives::PrintPretty);
    DefinePrimitive(context, "print-new-line",           Lisp::Primitives::PrintNewLine);
    DefinePrimitive(context, "load",                     Lisp::Primitives::Load);
    DefinePrimitive(context, "read-line",                Lisp::Primitives::ReadLine);
}

void AddArgs(Lisp::Context& context, int argc, char* argv[])
{
    std::vector<Lisp::Value> values;
    values.reserve(argc);

    for (int i = 0; i < argc; ++i)
    {
        Lisp::Util::StringWindow window(argv[i]);
        Lisp::GcPtr<Lisp::Object> str = context.GetStringsInterner().Intern(window);
        values.push_back(Lisp::Value(str));
    }

    context.Define(Lisp::Util::StringWindow("*args*"), Lisp::Funcs::List(context, values));
}

void DefinePrimitive(Lisp::Context& context, const char* name, Lisp::PrimitiveFn fn)
{
    context.DefinePrimitive(Lisp::Util::StringWindow(name), fn);
}

void DefineSpecialForm(Lisp::Context& context, const char* name, Lisp::SpecialFormFn fn);

void AddSpecialForms(Lisp::Context& context)
{
    DefineSpecialForm(context, "quote", Lisp::Quote);
    DefineSpecialForm(context, "if", Lisp::If);
    DefineSpecialForm(context, "execute", Lisp::Execute);
    DefineSpecialForm(context, "lambda", Lisp::Lambda);
    DefineSpecialForm(context, "macro", Lisp::Macro);
    DefineSpecialForm(context, "define", Lisp::Define);
    DefineSpecialForm(context, "let-one", Lisp::LetOne);
    DefineSpecialForm(context, "macro-expand", Lisp::MacroExpand);
    DefineSpecialForm(context, "cond", Lisp::Cond);
    DefineSpecialForm(context, "redefine", Lisp::Redefine);
}

void DefineSpecialForm(Lisp::Context& context, const char* name, Lisp::SpecialFormFn fn)
{
    context.DefineSpecialForm(Lisp::Util::StringWindow(name), fn);
}

void LoadPrelude(Lisp::Context& context)
{
    Lisp::Funcs::Load(context, "prelude.lisp");
}
