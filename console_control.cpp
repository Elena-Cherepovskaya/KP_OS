
#include "console_control.hpp"
#include "console_IO.hpp"
#include "T_Btree_memory.hpp"

#include <vector>


console_control::console_control(I_IO *IO, I_container *container)
    : _IO(IO)
    , _container(container)
{

}

void console_control::update()
{
    while (true)
    {
        std::string command_str;
        if (_IO->get_line(command_str))
        {
            if (command_str == "quit")
                break;

            parse(command_str);
        }
    }
}

void console_control::parse(const std::string &command_str)
{
    std::vector<std::string> data_vector;
    std::string tmp_str;

    for (auto i = 0; i < command_str.size(); ++i)
    {
        if (command_str[i] == '|')
        {
            data_vector.push_back(tmp_str);
            tmp_str.clear();

            continue;
        }

        tmp_str.push_back(command_str[i]);
    }

    switch (data_vector.size())
    {
        case 1:
        {
            _container->add_pool(data_vector[0]);
            break;
        }
        case 2:
        {
            _container->add_schema(data_vector[0], data_vector[1]);
            break;
        }
        case 3:
        {
            _container->add_collection(data_vector[0], data_vector[1], data_vector[2]);
            break;
        }
        case 5:
        {
            _container->add(data_vector[0], data_vector[1], data_vector[2], data_vector[3], data_vector[4]);
            break;
        }
        default:
            break;
    }

    data_vector.clear();
}