#include "Utilities.h"

#include <algorithm>

namespace nh
{
    std::string nh::ToLower(const std::string& string)
    {
        std::string s = string;

        std::transform(string.begin(), string.end(), s.begin(), [](char c)
        {
            return std::tolower(c);
        });

        return s;
    }

    bool ICompare(const std::string& string1, const std::string& string2)
    {
        if (string1.length() != string2.length()) { return false; }

        return std::equal(string1.begin(), string1.end(), string2.begin(), [](char c1, char c2)
        {
            return (std::tolower(c1) == std::tolower(c2));
        });
    }

    std::string UniqueString(const std::string& string)
    {
        static int i = 0;

        return string + std::to_string(i++);
    }
}
