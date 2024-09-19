

#ifndef list_command_hpp
#define list_command_hpp

#include "I_container.hpp"

#include <memory>
#include <time.h>
#include <list>

class list_command : I_container
{
    class data_list_command
    {
    public:
        data_list_command()
        {
            _time = time(NULL);
        }
        enum action_type
        {
            add_pool,
            remove_pool,
            add_schema,
            remove_schema,
            add_collection,
            remove_collection,
            add,
            update,
            remove
        };

        time_t _time;
        action_type action;

        std::string pool_key;
        std::string schema_key;
        std::string collection_key;
        std::string key;

        //Значение, которое вставили
        //Значение, которое заменили
        std::optional<std::string> value;
        //Значение, на которое заменили
        std::optional<std::string> new_value;
    };

    typedef std::shared_ptr<data_list_command> data_list_command_ptr;
    std::list<data_list_command_ptr> _list;

    std::list<data_list_command_ptr>::iterator _cur_iterator;

public:
    list_command(I_container *);
    ~list_command() override = default;

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

    void goto_time(time_t time);

    void go_original();

private:
    I_container *_base;
};


#endif /* list_command_hpp */
