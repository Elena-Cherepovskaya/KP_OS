
#include "string_pool.hpp"

string_pool::string_pool(std::string const & str)
{
    auto it = _pool.find(str);
    if (it == _pool.end())
    {
        auto res = std::make_shared<std::string>(str);
        _str = res;
        _pool[str] = res;
    }
    else
        _str = it->second;
}

string_pool::operator std::string const &() const
{
    return *_str;
}

bool string_pool::operator< (std::string const & str) const
{
    return *_str < str;
}

bool string_pool::operator> (std::string const & str) const
{
    return *_str > str;
}

std::map<std::string, std::shared_ptr<std::string>> string_pool::_pool;
