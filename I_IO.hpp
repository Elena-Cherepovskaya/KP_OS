

#ifndef KP_I_IO_HPP
#define KP_I_IO_HPP

#include <string>

class I_IO
{
protected:
    I_IO() = default;

public:
    virtual ~I_IO() = default;

    virtual bool get_line(std::string &) = 0;
    virtual void put_line(std::string const &) = 0;
};

#endif //KP_I_IO_HPP
