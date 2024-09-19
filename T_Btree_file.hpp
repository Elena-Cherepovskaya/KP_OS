
#ifndef KP_T_BTREE_FILE_HPP
#define KP_T_BTREE_FILE_HPP

#include "T_Btree_memory.hpp"

#include <fstream>
#include <string>

class T_Btree_file : public T_Btree_memory
{
public:

    T_Btree_file() = default;

    T_Btree_file(std::string const &);

    ~T_Btree_file();


    void output();

    void output(std::vector<std::string> & node_vector, B_node_pool_tree_ptr);
    void output(std::vector<std::string> & node_vector, B_node_schema_tree_ptr);
    void output(std::vector<std::string> & node_vector, B_node_collection_tree_ptr);
    void output(std::vector<std::string> & node_vector, B_node_data_tree_ptr);

    void output_node_vector(std::vector<std::string> & node_vector);

    std::ostream * _stream = &std::cout;

};
#endif //KP_T_BTREE_FILE_HPP
