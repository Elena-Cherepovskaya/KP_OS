

#ifndef KP_EXCEPTION_BTREE_MEMORY_HPP
#define KP_EXCEPTION_BTREE_MEMORY_HPP

#include <exception>

class Exception_Btree_memory : public std::exception
{
private:
    char const * _str;

public:
    Exception_Btree_memory(char const * str)
            : _str(str)
    {

    }
};

#endif //KP_EXCEPTION_BTREE_MEMORY_HPP
