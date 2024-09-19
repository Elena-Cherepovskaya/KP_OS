

#ifndef KP_I_CONTAINER_HPP
#define KP_I_CONTAINER_HPP

#include <string>
#include <vector>
#include <optional>

class I_container
{
public:
    virtual ~I_container() = default;

    virtual void add_pool(std::string const & pool_key) = 0;
    virtual void remove_pool(std::string const & pool_key) = 0;

    virtual void add_schema(std::string const & pool_key, std::string const & schema_key) = 0;
    virtual void remove_schema(std::string const & pool_key, std::string const & schema_key) = 0;

    virtual void add_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key) = 0;
    virtual void remove_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key) = 0;

    virtual void add(std::string const & pool_key,
                     std::string const & schema_key,
                     std::string const & collection_key,
                     std::string const & key,
                     std::string const & value) = 0;

    virtual std::optional<std::string> read(std::string const & pool_key,
                     std::string const & schema_key,
                     std::string const & collection_key,
                     std::string const & key) = 0;

    virtual std::optional<std::vector<std::string>> read_in_range(std::string const & pool_key,
                                                          std::string const & schema_key,
                                                          std::string const & collection_key,
                                                          std::string const & minbound,
                                                          std::string const & maxbound) = 0;

    virtual void update(std::string const & pool_key,
                        std::string const & schema_key,
                        std::string const & collection_key,
                        std::string const & key,
                        std::string const & value) = 0;

    virtual void remove(std::string const & pool_key,
                        std::string const & schema_key,
                        std::string const & collection_key,
                        std::string const & key) = 0;

};
#endif //KP_I_CONTAINER_HPP
