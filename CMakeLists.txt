cmake_minimum_required(VERSION 3.28)
project(KP)

set(CMAKE_CXX_STANDARD 17)

add_executable(KP main.cpp
        T_Btree_memory.cpp
        T_Btree_file.cpp
        console_IO.cpp
        console_control.cpp
        string_pool.cpp
        list_command.cpp
        list_command.cpp
)
