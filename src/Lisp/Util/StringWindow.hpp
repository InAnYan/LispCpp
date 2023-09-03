#ifndef LISP_UTIL_STRINGWINDOW_HPP
#define LISP_UTIL_STRINGWINDOW_HPP

#include <string>
#include <functional>
#include <charconv>

namespace Lisp
{
    namespace Util
    {
        /// A class that holds two string iterators. Similar to `std::string_view`.
        /// WARNING: This class won't check the correctnes of the iterators.
        class StringWindow
        {
        public:
            StringWindow(std::string::const_iterator beginIt, std::string::const_iterator endIt);
            explicit StringWindow(const std::string& str);
            explicit StringWindow(const char* literal); // If we were in Rust, we would write 'static to ensure, that this pointer points to the real string literal.
            
            // A union! Scared?
            union
            {
                // Wait, but those are public fields. Even more scared?
                std::string::const_iterator begin;
                std::string::const_iterator start;
            };

            union
            {
                std::string::const_iterator end;
                std::string::const_iterator current;
            };

            std::string ToCppString() const;

            std::size_t Length() const
            {
                return end - begin;
            }

            template <typename T>
            std::from_chars_result Parse(T& result) const
            {
                return std::from_chars(begin.base(), begin.base() + Length(), result);
            }

            /// Check that the two substrings are equal.
            /// This function doesn't compare iterators for equality, rather the contents.
            bool operator==(const StringWindow& other) const;
        }; // class StringWindow
    }
}

template <>
struct std::hash<Lisp::Util::StringWindow>
{
    // FNV-1a
    std::size_t operator()(const Lisp::Util::StringWindow& window) const
    {
        // I'm not quite sure this is the right way to do it, beacause of `std::size_t` and `uint32_t` conversion.
                
        uint32_t hash = 2166136261u;
        uint32_t prime = 16777619;

        for (std::string::const_iterator it = window.begin; it != window.end; ++it) {
            hash ^= static_cast<uint8_t>(*it);
            hash *= prime;
        }

        return hash;
    }
};

#endif // LISP_UTIL_STRINGWINDOW_HPP
