#include "utils.hpp"

namespace utils
{
    const string toLower(const string& str)
    {
        string res;
        res.resize(str.size());

        for (size_t i = 0; i < str.size(); i++)
        {
            res[i] = std::tolower(str[i]);
        }
        return res;
    }
}
