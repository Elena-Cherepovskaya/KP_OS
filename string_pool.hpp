
#ifndef string_pool_hpp
#define string_pool_hpp

#include <memory>
#include <string>
#include <map>

class string_pool
{
public:
    string_pool() = default;
    string_pool(std::string const & str);
    ~string_pool() = default;

    operator std::string const & () const;

    bool operator< (std::string const & str) const;
    bool operator> (std::string const & str) const;

private:
    std::shared_ptr<std::string> _str;

    static std::map<std::string, std::shared_ptr<std::string>> _pool;
};

#endif /* string_pool_hpp */
