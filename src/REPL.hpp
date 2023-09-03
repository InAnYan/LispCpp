#ifndef REPL_HPP
#define REPL_HPP

#include <iostream>
#include <string>

#include "Lisp/Context.hpp"

class REPL
{
public:
    explicit REPL(Lisp::Context& context);

    int Start();

private:
    Lisp::Context& context;

    void RunLoop();
    void ExecuteLine(const std::string& line);
}; // class REPL

#endif // REPL_HPP
