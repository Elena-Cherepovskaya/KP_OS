

#include "list_command.hpp"

list_command::list_command(I_container * base)
        : _base(base)
        , _cur_iterator(_list.end())
{
}

void list_command::add_pool(std::string const & pool_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::add_pool;
    action->pool_key = pool_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->add_pool(pool_key);
}

void list_command::remove_pool(std::string const & pool_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::remove_pool;
    action->pool_key = pool_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->remove_pool(pool_key);
}

void list_command::add_schema(std::string const & pool_key, std::string const & schema_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::add_schema;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->add_schema(pool_key, schema_key);
}

void list_command::remove_schema(std::string const & pool_key, std::string const & schema_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::remove_schema;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->remove_schema(pool_key, schema_key);
}

void list_command::add_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::add_collection;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    action->collection_key = collection_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->add_collection(pool_key, schema_key, collection_key);
}

void list_command::remove_collection(std::string const & pool_key, std::string const & schema_key, std::string const & collection_key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::remove_collection;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    action->collection_key = collection_key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->remove_collection(pool_key, schema_key, collection_key);
}

void list_command::add(std::string const & pool_key,
                       std::string const & schema_key,
                       std::string const & collection_key,
                       std::string const & key,
                       std::string const & value)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::add;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    action->collection_key = collection_key;
    action->key = key;
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->add(pool_key, schema_key, collection_key, key, value);
}

std::optional<std::string> list_command::read(std::string const & pool_key,
                                              std::string const & schema_key,
                                              std::string const & collection_key,
                                              std::string const & key)
{
    return _base->read(pool_key, schema_key, collection_key, key);
}

std::optional<std::vector<std::string>> list_command::read_in_range(std::string const & pool_key,
                                                                    std::string const & schema_key,
                                                                    std::string const & collection_key,
                                                                    std::string const & minbound,
                                                                    std::string const & maxbound)
{
    return _base->read_in_range(pool_key, schema_key, collection_key, minbound, maxbound);
}

void list_command::update(std::string const & pool_key,
                          std::string const & schema_key,
                          std::string const & collection_key,
                          std::string const & key,
                          std::string const & value)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::update;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    action->collection_key = collection_key;
    action->key = key;
    action->value = _base->read(pool_key, schema_key, collection_key, key);
    action->new_value = value;

    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->update(pool_key, schema_key, collection_key, key, value);
}

void list_command::remove(std::string const & pool_key,
                          std::string const & schema_key,
                          std::string const & collection_key,
                          std::string const & key)
{
    data_list_command_ptr action = std::make_shared<data_list_command>();
    action->action = data_list_command::remove;
    action->pool_key = pool_key;
    action->schema_key = schema_key;
    action->collection_key = collection_key;
    action->key = key;
    action->value = _base->read(pool_key, schema_key, collection_key, key);
    _list.push_back(action);

    if (_cur_iterator == _list.end())
        _base->remove(pool_key, schema_key, collection_key, key);
}

void list_command::goto_time(time_t time)
{

    while (_cur_iterator != _list.begin())
    {
        --_cur_iterator;
        auto const & action =  **_cur_iterator;
        if (action._time > time)
        {
            switch (action.action)
            {
                case data_list_command::add_pool:
                    _base->remove_pool(action.pool_key);
                    break;

                case data_list_command::remove_pool:
                    _base->add_pool(action.pool_key);
                    break;

                case data_list_command::add_schema:
                    _base->remove_schema(action.pool_key, action.schema_key);
                    break;

                case data_list_command::remove_schema:
                    _base->add_schema(action.pool_key, action.schema_key);
                    break;

                case data_list_command::add_collection:
                    _base->remove_collection(action.pool_key, action.schema_key, action.collection_key);
                    break;

                case data_list_command::remove_collection:
                    _base->add_collection(action.pool_key, action.schema_key, action.collection_key);
                    break;

                case data_list_command::add:
                    _base->remove(action.pool_key, action.schema_key, action.collection_key, action.key);
                    break;

                case data_list_command::update:
                    _base->update(action.pool_key, action.schema_key, action.collection_key, action.key, action.value.value());
                    break;

                case data_list_command::remove:
                    if (action.value.has_value())
                        _base->add(action.pool_key, action.schema_key, action.collection_key, action.key, action.value.value());
                    break;
            }
        }
        else
            break;
    }
}

void list_command::go_original()
{
    while (_cur_iterator != _list.end())
    {
        auto const & action =  **_cur_iterator;

        switch (action.action)
        {
            case data_list_command::add_pool:
                _base->add_pool(action.pool_key);
                break;

            case data_list_command::remove_pool:
                _base->remove_pool(action.pool_key);
                break;

            case data_list_command::add_schema:
                _base->add_schema(action.pool_key, action.schema_key);
                break;

            case data_list_command::remove_schema:
                _base->remove_schema(action.pool_key, action.schema_key);
                break;

            case data_list_command::add_collection:
                _base->add_collection(action.pool_key, action.schema_key, action.collection_key);
                break;

            case data_list_command::remove_collection:
                _base->remove_collection(action.pool_key, action.schema_key, action.collection_key);
                break;

            case data_list_command::add:
                _base->add(action.pool_key, action.schema_key, action.collection_key, action.key, action.value.value());
                break;

            case data_list_command::update:
                _base->update(action.pool_key, action.schema_key, action.collection_key, action.key, action.new_value.value());
                break;

            case data_list_command::remove:
                if (action.value.has_value())
                    _base->remove(action.pool_key, action.schema_key, action.collection_key, action.key);
                break;

        }

        ++_cur_iterator;
    }
}
