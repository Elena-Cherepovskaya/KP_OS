

#ifndef KP_EXCEPTION_FILE_HPP
#define KP_EXCEPTION_FILE_HPP

#include <exception>

#include "string"

class Exception_file : public std::exception
{
private:
    char const * _str;

public:
    Exception_file(char const * str)
            : _str(str)
    {

    }
};

#endif //KP_EXCEPTION_FILE_HPP
