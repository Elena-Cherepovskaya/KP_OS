

#include "T_Btree_memory.hpp"
#include "BTree.hpp"

#include "Exception_Btree_memory.hpp"

T_Btree_memory::T_Btree_memory()
{
    _data_pool = std::make_shared<pool_tree>();
}

void T_Btree_memory::add_pool(const std::string &pool_key)
{
    _data_pool->insert(pool_key, std::make_shared<schema_tree>());
}

void T_Btree_memory::remove_pool(const std::string &pool_key)
{
    _data_pool->remove(pool_key);
}

void T_Btree_memory::add_schema(std::string const & pool_key, std::string const & schema_key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path;
    _data_pool->find_path(path, pool_key);

    if (path.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path.top().first->element_list.at(path.top().second).second->insert(schema_key, std::make_shared<collection_tree>());
}

void T_Btree_memory::remove_schema(const std::string &pool_key, const std::string &schema_key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path;
    _data_pool->find_path(path, pool_key);

    if (path.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path.top().first->element_list.at(path.top().second).second->remove(schema_key);
}

void T_Btree_memory::add_collection(const std::string &pool_key, const std::string &schema_key,
                                    const std::string &collection_key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path_schema.top().first->element_list.at(path_schema.top().second).second->insert(collection_key, std::make_shared<data_tree>());
}

void T_Btree_memory::remove_collection(std::string const & pool_key, std::string const & schema_key,
                                       std::string const & collection_key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path_schema.top().first->element_list.at(path_schema.top().second).second->remove(collection_key);
}


void T_Btree_memory::add(const std::string &pool_key, const std::string &schema_key, const std::string &collection_key,
                         const std::string &key, const std::string &value)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_collection_tree_ptr, int>> path_collection;
    path_schema.top().first->element_list.at(path_schema.top().second).second->find_path(path_collection, collection_key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path_collection.top().first->element_list.at(path_collection.top().second).second->insert(key, value);
}

std::optional<std::string> T_Btree_memory::read(const std::string &pool_key, const std::string &schema_key, const std::string &collection_key,
                          const std::string &key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_collection_tree_ptr, int>> path_collection;
    path_schema.top().first->element_list.at(path_schema.top().second).second->find_path(path_collection, collection_key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_data_tree_ptr, int>> path_data;
    path_collection.top().first->element_list.at(path_collection.top().second).second->find_path(path_data, key);

    if (path_data.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    return path_data.top().first->element_list.at(path_data.top().second).second;
}


void T_Btree_memory::_read_in_range(std::vector<std::pair<std::string, std::string>> & node_vector,
                                    B_node_data_tree_ptr subtree_root)
{
    if (subtree_root == nullptr)
    {
        return;
    }

    for (auto i = 0; i < subtree_root->keys_count; ++i)
    {
        node_vector.push_back(std::make_pair(subtree_root->element_list[i].first, subtree_root->element_list[i].second));
    }

    for (auto i = 0; i <= subtree_root->keys_count; ++i)
    {
        _read_in_range(node_vector, subtree_root->child_list[i]);
    }
}


std::optional<std::vector<std::string>> T_Btree_memory::read_in_range(const std::string &pool_key,
                                                                      const std::string &schema_key,
                                                                      const std::string &collection_key,
                                                                      const std::string &minbound,
                                                                      const std::string &maxbound)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_collection_tree_ptr, int>> path_collection;
    path_schema.top().first->element_list.at(path_schema.top().second).second->find_path(path_collection, collection_key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::vector<std::pair<std::string, std::string>> node_vector;

    auto subtree_root = path_collection.top().first->element_list.at(path_collection.top().second).second;

    _read_in_range(node_vector, subtree_root->get_root());

    std::vector<std::string> res_vector;
    for (auto i = 0; i < node_vector.size(); ++i)
    {
        if (node_vector[i].first >= minbound && node_vector[i].first <= maxbound)
            res_vector.push_back(node_vector[i].second);
    }

    if (res_vector.size() > 0)
        return res_vector;

    return std::nullopt;
}

void T_Btree_memory::update(const std::string &pool_key, const std::string &schema_key,
                            const std::string &collection_key, const std::string &key, const std::string &value)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_collection_tree_ptr, int>> path_collection;
    path_schema.top().first->element_list.at(path_schema.top().second).second->find_path(path_collection, collection_key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_data_tree_ptr, int>> path_data;
    path_collection.top().first->element_list.at(path_collection.top().second).second->find_path(path_data, key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path_data.top().first->element_list.at(path_data.top().second).second = value;
}

void T_Btree_memory::remove(const std::string &pool_key, const std::string &schema_key,
                            const std::string &collection_key, const std::string &key)
{
    std::stack<std::pair<B_node_pool_tree_ptr, int>> path_pool;
    _data_pool->find_path(path_pool, pool_key);

    if (path_pool.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_schema_tree_ptr, int>> path_schema;
    path_pool.top().first->element_list.at(path_pool.top().second).second->find_path(path_schema, schema_key);

    if (path_schema.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    std::stack<std::pair<B_node_collection_tree_ptr, int>> path_collection;
    path_schema.top().first->element_list.at(path_schema.top().second).second->find_path(path_collection, collection_key);

    if (path_collection.top().second < 0)
    {
        throw Exception_Btree_memory("invalid_key");
    }

    path_collection.top().first->element_list.at(path_collection.top().second).second->remove(key);
}