#pragma once

#include <vector>
#include <string>

class StringUtil
{
public:
    static std::string trim(const std::string& str, const std::vector<char>& uselessChar)
    {
        std::string result;

        for (const auto& c : str)
        {
            bool shouldTrim = false;
            for (const auto& u : uselessChar)
            {
                if (c == u)
                {
                    shouldTrim = true;
                    break;
                }
            }

            if (shouldTrim)
                continue;

            result += c;
        }

        return result;
    }

    static std::vector<std::string> split(const std::string& str, const std::vector<char>& separator)
    {
        if (str.empty())
            return std::vector<std::string>();

        std::vector<std::string> result(1, "");

        int i = 0;
        for (const auto& c : str)
        {
            bool shouldSplit = false;
            for (const auto& s : separator)
            {
                if (c == s)
                {
                    shouldSplit = true;
                    break;
                }
            }

            if (shouldSplit)
            {
                ++i;
                result.emplace_back("");
                continue;
            }

            result[i] += c;
        }

        return result;
    }
};