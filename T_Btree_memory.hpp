

#ifndef KP_T_BTREE_MEMORY_HPP
#define KP_T_BTREE_MEMORY_HPP

#include "I_container.hpp"
#include "BTree.hpp"
#include "forward.hpp"

class T_Btree_memory : public I_container
{
public:

    T_Btree_memory();

    ~T_Btree_memory() override = default;

    void add_pool(std::string const & pool_key) override;
    void remove_pool(std::string const & pool_key) override;

    void add_schema(std::string const & pool_key, std::string const & schema_key) override;
    void remove_schema(std::string const & pool_key, std::string const & schema_key) override;

    void add_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key) override;
    void remove_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key) override;

    void add(std::string const & pool_key,
                     std::string const & schema_key,
                     std::string const & collection_key,
                     std::string const & key,
                     std::string const & value) override;

    std::optional<std::string> read(std::string const & pool_key,
                             std::string const & schema_key,
                             std::string const & collection_key,
                             std::string const & key) override;

private:

    void _read_in_range(std::vector<std::pair<std::string, std::string>> & node_vector,
                        B_node_data_tree_ptr subtree_root);

public:

    std::optional<std::vector<std::string>> read_in_range(std::string const & pool_key,
                                                   std::string const & schema_key,
                                                   std::string const & collection_key,
                                                   std::string const & minbound,
                                                   std::string const & maxbound) override;

    void update(std::string const & pool_key,
                        std::string const & schema_key,
                        std::string const & collection_key,
                        std::string const & key,
                        std::string const & value) override;

    void remove(std::string const & pool_key,
                        std::string const & schema_key,
                        std::string const & collection_key,
                        std::string const & key) override;


protected:
    pool_tree_ptr _data_pool;

};
#endif //KP_T_BTREE_MEMORY_HPP
