#include "Reader.hpp"

#include <stdexcept>
#include <charconv>
#include <cassert>
#include <system_error>

#include "Configuration.hpp"

#include "Funcs/List.hpp"

namespace Lisp
{
    class NoLispObjects : std::exception
    {
        
    };

    bool IsDigit(char ch);
    bool IsWhite(char ch);
    bool IsSymbol(char ch);

    Reader::Reader(Context& context, Util::StringWindow input)
        : context(context), token(input.begin, input.begin), inputEnd(input.end)
    {
        
    }

    Value Reader::ReadValue()
    {
        try
        {
            return ReadValueRaw();
        }
        catch (NoLispObjects& e)
        {
            context.MakeAndRaiseErrorOnlyAbort("no-lisp-objects-found", "No Lisp objects found in input string.");
            return NIL;
        }
    }

    // May throw `NoLispObjects`.
    Value Reader::ReadValueRaw()
    {
        SkipWhitespace();
        return StartReadingValue();
    }

    Value Reader::ReadValues()
    {
        SkipWhitespace();
        return StartReadingValues();
    }
    
    void Reader::SkipWhitespace()
    {
        AdvanceWhile(IsWhite);
        SkipComment();
        BeginNewRead();
    }

    void Reader::SkipComment()
    {
        if (Match(';'))
        {
            AdvanceWhile([](char ch)
            {
                return ch != '\n';
            });

            SkipWhitespace();
        }
    }
    
    void Reader::BeginNewRead()
    {
        token.start = token.current;
    }

    Value Reader::StartReadingValue()
    {
        Value value;

        if (IsDigit(Peek()))
        {
            value = ReadNumber();
        }
        // A hack. And not a safe peek.
        else if (Peek() == '-' && IsDigit(Peek(1)))
        {
            Advance();
            value = ReadNumber();
        }
        else if (Match('"'))
        {
            value = ReadString();
        }
        else if (IsSymbol(Peek()))
        {
            value = ReadSymbol();
        }
        else if (Match('('))
        {
            value = ReadList();
        }
        else if (Match('\''))
        {
            return ReadQuote();
        }
        else
        {
            throw NoLispObjects();
        }

        if (Configuration::Reader::LOG)
        {
            std::cout << "--- Reader: Read " << value << std::endl;
        }

        return value;
    }

    Value Reader::StartReadingValues()
    {
        Value list = NIL;

        while (true)
        {
            try
            {
                Value value = ReadValueRaw();
                Funcs::Push(context, value, list);
            }
            catch (NoLispObjects& e)
            {
                break;
            }
        }

        list = Funcs::NReverse(list);

        if (Configuration::Reader::LOG)
        {
            std::cout << "--- Reader: Read " << list << std::endl;
        }

        return list;
    }

    Value Reader::ReadNumber()
    {
        AdvanceWhile(IsDigit);

        if (Match('.'))
        {
            return ContinueReadingFloat();
        }
        else
        {
            return ParseInteger();
        }
    }

    template <typename T>
    Value ParseNumber(Context& context, Util::StringWindow token)
    {
        T result;
        auto [ptr, ec] = token.Parse(result);

        if (ec == std::errc::invalid_argument)
        {
            assert(false && "Lisp::Reader does not recognize integer numbers correctly");
            return NIL;
        }
        else if (ec == std::errc::result_out_of_range)
        {
            context.MakeAndRaiseErrorOnlyAbort("number-out-of-range", "Found number that is out of representation range.");
            return NIL;
        }
        else
        {
            return Value(static_cast<T>(result));
        }
    }

    Value Reader::ParseInteger()
    {
        return ParseNumber<Integer>(context, token);
    }

    Value Reader::ContinueReadingFloat()
    {
        AdvanceWhile(IsDigit);

        return ParseFloat();
    }

    Value Reader::ParseFloat()
    {
        return ParseNumber<Float>(context, token);
    }

    Value Reader::ReadString()
    {
        AdvanceWhile([](char ch)
        {
            return ch != '"';
        });

        if (IsAtEnd())
        {
            context.MakeAndRaiseErrorOnlyAbort("unterminated-string", "Unterminated string found.");
            return NIL;
        }

        Advance();

        return MakeString();
    }

    Value Reader::ReadList()
    {
        Value values = ReadValues();

        if (!Match(')'))
        {
            context.MakeAndRaiseErrorOnlyAbort("unterminated-string-list", "Unterminated list found.");
            return NIL;
        }

        return values;
    }

    Value Reader::ReadQuote()
    {
        Value value = ReadValue();

        return Funcs::List(context, std::initializer_list<Value>{ context.InternSymbol(Util::StringWindow("quote")), value });
    }

    Value Reader::MakeString()
    {
        Util::StringWindow stringWindow(token.begin + 1, token.end - 1);

        return Value(context.GetStringsInterner().Intern(stringWindow));
    }

    Value Reader::ReadSymbol()
    {
        AdvanceWhile(IsSymbol);

        return context.InternSymbol(token);
    }

    char Reader::Peek(int offset) const
    {
        return token.current[offset];
    }

    void Reader::AdvanceWhile(std::function<bool(char)> fn)
    {
        while (!IsAtEnd())
        {
            if (fn(Peek()))
            {
                Advance();
            }
            else
            {
                break;
            }
        }
    }

    bool Reader::IsAtEnd() const
    {
        return token.current == inputEnd;
    }

    char Reader::Advance()
    {
        ++token.current;
        return Peek(-1);
    }

    bool Reader::Match(char ch)
    {
        if (Peek() == ch)
        {
            Advance();
            return true;
        }
        else
        {
            return false;
        }
    }

    bool InRange(char ch, char a, char b);

    bool IsDigit(char ch)
    {
        return InRange(ch, '0', '9');
    }

    bool InRange(char ch, char a, char b)
    {
        return ch >= a && ch <= b;
    }

    bool IsWhite(char ch)
    {
        switch (ch)
        {
        case ' ':
        case '\t':
        case '\n':
        case '\r':
            return true;
        default:
            return false;
        }
    }

    bool IsSymbol(char ch)
    {
        if (IsWhite(ch))
        {
            return false;
        }

        switch (ch)
        {
        case '"':
        case '(':
        case ')':
        case '\'':
        case '#':
        case '`':
        case ',':
        case '.':
        case '\0':
        case ';':
            return false;
        default:
            return true;
        }
    }
}
