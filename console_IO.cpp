

#include "console_IO.hpp"

#include <iostream>

bool console_IO::get_line(std::string & line)
{
    std::cin >> line;
    return !line.empty();
}

void console_IO::put_line(std::string const & line)
{
    std::cout << line << std::endl;
}
