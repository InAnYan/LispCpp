#ifndef LISP_READER_HPP
#define LISP_READER_HPP

#include "Util/StringWindow.hpp"

#include "Value.hpp"
#include "Context.hpp"

namespace Lisp
{
    class Reader
    {
    public:
        Reader(Context& context, Util::StringWindow input);
        
        /// Read the first value. If there is some symbols after that value, leave them as is.
        Value ReadValue();
        /// Read a sequence of values into list. Stop at the end of the input or at the unpaired ')'.
        Value ReadValues();

    private:
        Context& context;

        Util::StringWindow token;
        std::string::const_iterator inputEnd;

        Value ReadValueRaw();

        void SkipWhitespace();
        void SkipComment();
        void BeginNewRead();

        Value StartReadingValue();
        Value StartReadingValues();

        Value ReadNumber();
        Value ParseInteger();

        Value ContinueReadingFloat();
        Value ParseFloat();

        Value ReadString();
        Value MakeString();

        Value ReadSymbol();

        Value ReadList();

        Value ReadQuote();

        char Peek(int offset = 0) const;
        void AdvanceWhile(std::function<bool(char)> fn);
        bool IsAtEnd() const;
        char Advance();
        bool Match(char ch);

        std::size_t MakeOffset() const;
    }; // class Reader
}

#endif // LISP_READER_HPP
