

#include "T_Btree_file.hpp"

#include <fstream>

#include "Exception_file.hpp"

T_Btree_file::T_Btree_file(const std::string &fin_name)
{
    std::ifstream fin;
    fin.open(fin_name);

    if (!fin.is_open())
    {
        throw Exception_file("File_is_nod_found");
    }

    while(!fin.eof())
    {
        std::string cur_str;
        std::getline(fin, cur_str);

        std::string tmp_str;
        std::vector<std::string> data_vector;

        for (auto i = 0; i < cur_str.size(); ++i)
        {
            if (cur_str[i] == '|')
            {
                data_vector.push_back(tmp_str);
                tmp_str.clear();
                continue;
            }

            tmp_str.push_back(cur_str[i]);
        }

        try
        {
            switch (data_vector.size())
            {
                case 1:
                {
                    T_Btree_memory::add_pool(data_vector[0]);
                    break;
                }
                case 2:
                {
                    T_Btree_memory::add_schema(data_vector[0], data_vector[1]);
                    break;
                }
                case 3:
                {
                    T_Btree_memory::add_collection(data_vector[0], data_vector[1], data_vector[2]);
                    break;
                }
                case 5:
                {
                    T_Btree_memory::add(data_vector[0], data_vector[1], data_vector[2], data_vector[3], data_vector[4]);
                    break;
                }
                default:
                    break;
            }

        }
        catch (Exception_Btree const &)
        {
        }

        data_vector.clear();
    }

    fin.close();

    _stream = new std::ofstream(fin_name);

}

T_Btree_file::~T_Btree_file()
{
    output();
    delete _stream;
}

void T_Btree_file::output()
{
    std::vector<std::string> node_vector;
    output(node_vector, _data_pool->get_root());
}

void T_Btree_file::output(std::vector<std::string> & node_vector, B_node_pool_tree_ptr node)
{
    if (node == nullptr)
    {
        output_node_vector(node_vector);
        return;
    }

    for (auto i = 0; i < node->keys_count; ++i)
    {
        node_vector.push_back(node->element_list[i].first);
        output_node_vector(node_vector);
        auto tree = node->element_list[i].second;
        output(node_vector, tree->get_root());
        node_vector.pop_back();
    }

    for(auto ci = 0; ci < node->child_list.size(); ++ci)
    {
        auto data = node->child_list[ci];
        output(node_vector, data);
    }
}

void T_Btree_file::output(std::vector<std::string> & node_vector, B_node_schema_tree_ptr node)
{
    if (node == nullptr)
    {
        output_node_vector(node_vector);
        return;
    }

    for (auto i = 0; i < node->keys_count; ++i)
    {
        node_vector.push_back(node->element_list[i].first);
        output_node_vector(node_vector);
        auto tree = node->element_list[i].second;
        output(node_vector, tree->get_root());
        node_vector.pop_back();
    }

    for(auto ci = 0; ci < node->child_list.size(); ++ci)
    {
        auto data = node->child_list[ci];
        output(node_vector, data);
    }
}

void T_Btree_file::output(std::vector<std::string> & node_vector, B_node_collection_tree_ptr node)
{
    if (node == nullptr)
    {
        output_node_vector(node_vector);
        return;
    }

    for (auto i = 0; i < node->keys_count; ++i)
    {
        node_vector.push_back(node->element_list[i].first);
        output_node_vector(node_vector);
        auto tree = node->element_list[i].second;
        output(node_vector, tree->get_root());
        node_vector.pop_back();
    }

    for(auto ci = 0; ci < node->child_list.size(); ++ci)
    {
        auto data = node->child_list[ci];
        output(node_vector, data);
    }
}

void T_Btree_file::output(std::vector<std::string> & node_vector, B_node_data_tree_ptr node)
{
    if (node == nullptr)
        return;

    for (auto i = 0; i < node->keys_count; ++i)
    {
        node_vector.push_back(node->element_list[i].first);

        node_vector.push_back(node->element_list[i].second);
        output_node_vector(node_vector);
        node_vector.pop_back();

        node_vector.pop_back();
    }

    for (auto ci = 0; ci < node->child_list.size(); ++ci)
    {
        auto data = node->child_list[ci];
        output(node_vector, data);
    }
}

void T_Btree_file::output_node_vector(std::vector<std::string> & node_vector)
{
    for (auto i = 0; i < node_vector.size(); ++i)
        *_stream << node_vector[i] << "|";

    *_stream << std::endl;
}
