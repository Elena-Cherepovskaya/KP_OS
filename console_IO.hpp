

#ifndef KP_CONSOLE_IO_HPP
#define KP_CONSOLE_IO_HPP

#include "I_IO.hpp"

class console_IO : public I_IO
{
public:
    console_IO() = default;
    ~console_IO() override = default;

    bool get_line(std::string &) override;
    void put_line(std::string const &) override;
};

#endif //KP_CONSOLE_IO_HPP
