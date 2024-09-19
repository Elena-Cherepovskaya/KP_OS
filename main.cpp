#include <iostream>
#include <fstream>
#include <sstream>

#include "BTree.hpp"
#include "T_Btree_memory.hpp"
#include "T_Btree_file.hpp"
#include "console_control.hpp"
#include "console_IO.hpp"
#include "list_command.hpp"

void check_insert_split()
{
    Btree<int, std::string, 8> tree([](int const &left, int const &right){ return left - right; });
    std::ofstream stream("b_tree_demo.txt");
    std::ofstream *stream_ptr = &stream; // = &std::cout;

    std::vector<int> keys(10000);
    for (auto i = 0; i < 10000; ++i)
    {
        keys[i] = i;
    }

    for (auto i = 0; i < 10000; ++i)
    {
        try
        {
            tree.insert(keys[i], std::to_string(i));

            if ((i + 1) % 100 == 0)
            {
                std::cout << "Iteration #" << i + 1 << " reached" << std::endl;
            }

            *stream_ptr << "Tree after insertion #" << i + 1 << " of " << keys[i] << ":" << std::endl;
            tree.print(*stream_ptr);
            *stream_ptr << std::endl;
        }
        catch (std::exception const &ex)
        {
            *stream_ptr << "Insertion exception occured!" << std::endl << std::endl;
        }
    }

    for (auto i = 0; i < 10000; ++i)
    {
        try
        {
            tree.remove(keys[i]);

            if ((i + 1) % 100 == 0)
            {
                std::cout << "Iteration #" << i + 1 << " reached" << std::endl;
            }

            *stream_ptr << "Tree after removal #" << i + 1 << " of " << keys[i] << ":" << std::endl;
            tree.print(*stream_ptr);
            *stream_ptr << std::endl;
        }
        catch (std::exception const &ex)
        {
            *stream_ptr << "Removal exception occured!" << std::endl << std::endl;
        }
    }
}

void Btree_tests()
{
    check_insert_split();
}

int main(int argc, char * argv[])
{
    std::string mode(argv[1]);

    I_container * tree;
    if (mode == "-f")
    {
        tree = new T_Btree_file(argv[2]);
    }
    else
    {
        tree = new T_Btree_memory();

        std::vector<std::string> vector_pool = {"pool_1",
                                                "pool_2",
                                                "pool_3"};

        for (auto i = 0; i < vector_pool.size(); ++i)
            tree->add_pool(vector_pool[i]);

        std::vector<std::string> vector_schema = {"schema_1",
                                                  "schema_2",
                                                  "schema_3"};

        for (auto i = 0; i < vector_pool.size(); ++i)
        {
            for (auto j = 0; j < 3; ++j)
            {
                tree->add_schema(vector_pool[i], vector_schema[j]);
            }
        }

        std::vector<std::string> vector_collection = {"collection_1",
                                                      "collection_2",
                                                      "collection_3"};

        for (auto i = 0; i < vector_pool.size(); ++i)
        {
            for (auto j = 0; j < vector_schema.size(); ++j)
            {
                for (auto k = 0; k < vector_collection.size(); ++k)
                {
                    tree->add_collection(vector_pool[i], vector_schema[j], vector_collection[k]);
                }
            }
        }

        std::vector<std::string> vector_key = {"key_1",
                                               "key_2",
                                               "key_3"};

        std::vector<std::string> vector_value = {"value_1",
                                                 "value_2",
                                                 "value_3"};

        for (auto i = 0; i < vector_pool.size(); ++i)
        {
            for (auto j = 0; j < vector_schema.size(); ++j)
            {
                for (auto k = 0; k < vector_collection.size(); ++k)
                {
                    for (auto t = 0; t < vector_key.size(); ++t)
                    {
                        tree->add(vector_pool[i], vector_schema[j], vector_collection[k], vector_key[t], vector_value[t]);
                    }
                }
            }
        }

        std::optional<std::string> a = tree->read(vector_pool[0], vector_schema[0], vector_collection[0], vector_key[0]);
        std::optional<std::string> b = tree->read(vector_pool[1], vector_schema[0], vector_collection[0], vector_key[0]);

        std::optional<std::vector<std::string>> vector_range;
        vector_range = tree->read_in_range(vector_pool[0],
                                          vector_schema[0],
                                          vector_collection[0],
                                          "key_1",
                                          "key_3");

        tree->update(vector_pool[0],
                    vector_schema[0],
                    vector_collection[0],
                    "key_1",
                    "value_4");
    }

    delete tree;


    I_container * cons_tree = new T_Btree_memory();
    I_IO * IO = new console_IO();

    console_control * console_test = new console_control(IO, cons_tree);

    console_test->update();

    delete IO;
    delete cons_tree;

    T_Btree_memory * test_tree = new T_Btree_memory();
    list_command * test_list = new list_command(test_tree);


    std::vector<std::string> vector_pool = {"pool_1",
                                            "pool_2",
                                            "pool_3"};

    for (auto i = 0; i < vector_pool.size(); ++i)
        tree->add_pool(vector_pool[i]);

    std::vector<std::string> vector_schema = {"schema_1",
                                              "schema_2",
                                              "schema_3"};

    for (auto i = 0; i < vector_pool.size(); ++i)
    {
        for (auto j = 0; j < 3; ++j)
        {
            test_list->add_schema(vector_pool[i], vector_schema[j]);
        }
    }

    std::vector<std::string> vector_collection = {"collection_1",
                                                  "collection_2",
                                                  "collection_3"};

    for (auto i = 0; i < vector_pool.size(); ++i)
    {
        for (auto j = 0; j < vector_schema.size(); ++j)
        {
            for (auto k = 0; k < vector_collection.size(); ++k)
            {
                test_list->add_collection(vector_pool[i], vector_schema[j], vector_collection[k]);
            }
        }
    }

    std::vector<std::string> vector_key = {"key_1",
                                           "key_2",
                                           "key_3"};

    std::vector<std::string> vector_value = {"value_1",
                                             "value_2",
                                             "value_3"};

    for (auto i = 0; i < vector_pool.size(); ++i)
    {
        for (auto j = 0; j < vector_schema.size(); ++j)
        {
            for (auto k = 0; k < vector_collection.size(); ++k)
            {
                for (auto t = 0; t < vector_key.size(); ++t)
                {
                    test_list->add(vector_pool[i], vector_schema[j], vector_collection[k], vector_key[t], vector_value[t]);
                }
            }
        }
    }

    test_list->update(vector_pool[0],
                 vector_schema[0],
                 vector_collection[0],
                 "key_1",
                 "value_4");

    test_list->update(vector_pool[0],
                      vector_schema[0],
                      vector_collection[0],
                      "key_2",
                      "value_5");

    test_list->update(vector_pool[0],
                      vector_schema[0],
                      vector_collection[0],
                      "key_3",
                      "value_6");

    test_list->go_original();

    delete test_tree;
    delete test_list;


    return 0;
}
