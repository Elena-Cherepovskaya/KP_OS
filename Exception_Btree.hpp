
#ifndef KP_EXCEPTION_BTREE_HPP
#define KP_EXCEPTION_BTREE_HPP

#include <exception>
#include <string>

class Exception_Btree : public std::exception
{
private:
    char const * _str;

public:
    Exception_Btree(char const * str)
    : _str(str)
    {

    }
};

#endif //KP_EXCEPTION_BTREE_HPP
