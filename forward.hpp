

#ifndef KP_FORWARD_HPP
#define KP_FORWARD_HPP

#include "BTree.hpp"

#include <string>

#include <string>

constexpr int _t = 2;


//typedef std::string string_data;

#include "string_pool.hpp"
typedef string_pool string_data;

typedef std::shared_ptr<Btree<string_data, string_data, _t>> Btree_ptr;

typedef Btree<string_data, string_data, _t> data_tree;
typedef std::shared_ptr<data_tree> data_tree_ptr;

typedef Btree<string_data, data_tree_ptr, _t> collection_tree;
typedef std::shared_ptr<collection_tree> collection_tree_ptr;

typedef Btree<string_data, collection_tree_ptr, _t> schema_tree;
typedef std::shared_ptr<schema_tree> schema_tree_ptr;

typedef Btree<string_data, schema_tree_ptr, _t> pool_tree;
typedef std::shared_ptr<pool_tree> pool_tree_ptr;

typedef B_node<string_data, schema_tree_ptr, _t> B_node_pool_tree;
typedef std::shared_ptr<B_node_pool_tree> B_node_pool_tree_ptr;

typedef B_node<string_data, collection_tree_ptr, _t> B_node_schema_tree;
typedef std::shared_ptr<B_node_schema_tree> B_node_schema_tree_ptr;

typedef B_node<string_data, data_tree_ptr, _t> B_node_collection_tree;
typedef std::shared_ptr<B_node_collection_tree> B_node_collection_tree_ptr;

typedef B_node<string_data, string_data, _t> B_node_data_tree;
typedef std::shared_ptr<B_node_data_tree> B_node_data_tree_ptr;

#endif //KP_FORWARD_HPP
