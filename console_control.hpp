

#ifndef KP_CONSOLE_CONTROL_HPP
#define KP_CONSOLE_CONTROL_HPP

#include <string>
#include <memory>

#include "I_IO.hpp"
#include "I_container.hpp"

class console_control
{
public:
    console_control(I_IO * IO,
                    I_container * container);

    void update();
    void parse(std::string const & command_str);

private:
    I_IO * _IO;
    I_container * _container;

};
#endif //KP_CONSOLE_CONTROL_HPP
