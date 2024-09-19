#ifndef UNTITLED_BTREE_HPP
#define UNTITLED_BTREE_HPP

#include <vector>
#include <memory>
#include <stack>
#include <iostream>
#include <functional>
#include <optional>
#include <memory>
#include <string>

#include "Exception_Btree.hpp"

template<typename T_key, typename T_value, int t>
class B_node
{
public:

    std::vector<std::pair<T_key, T_value>> element_list;

    std::vector<std::shared_ptr<B_node<T_key, T_value, t>>> child_list;
    int keys_count;

    B_node():
            element_list(2 * t - 1),
            child_list(2 * t),
            keys_count(0)
    {

    }

    //Вставляет элемент в узел по индексу
    void insert(
            std::pair<T_key, T_value> &&kvp,
            int index)
    {
        element_list[index] = std::move(kvp);
        ++keys_count;
    }

    [[nodiscard]] bool is_node_max_size_reached() const
    {
        return element_list.size() == (2 * t - 1);
    }
};

template<
        typename T_key,
        typename T_value,
        int t>
class Btree final
{
private:
    typedef std::shared_ptr<B_node<T_key, T_value, t>> B_node_ptr;

    B_node_ptr _root;
    std::function<int(T_key const &, T_key const &)> _comparer;

private:

    [[nodiscard]] constexpr int get_min_keys_count() const
    {
        return t - 1;
    }

    [[nodiscard]] constexpr int get_max_keys_count() const
    {
        return 2 * t - 1;
    }

public:

    explicit Btree(
            std::function<int(T_key const &, T_key const &)> comparer):
            _comparer(comparer),
            _root(nullptr)
    {

    }

    Btree()
    : _comparer(([](T_key const &left, T_key const &right)
    {
        std::less<T_key> cmp_less;
        if (cmp_less(left, right))
            return -1;

        std::greater<T_key> cmp_greater;
        if (cmp_greater(left, right))
            return 1;

        return 0;
    }))
    , _root(nullptr)
    {}

    B_node_ptr get_root()
    {
        return _root;
    }

    B_node_ptr copy_node(
            B_node_ptr const &other_node)
    {
        if (other_node == nullptr)
        {
            return nullptr;
        }

        B_node_ptr new_node = std::make_shared<B_node<T_key, T_value, t>>();
        new_node->element_list = other_node->element_list;
        new_node->keys_count = other_node->keys_count;

        for (int i = 0; i < other_node->child_list.size(); ++i)
        {
            new_node->child_list[i] = copy_node(other_node->child_list[i]);
        }

        return new_node;
    }

    Btree(
            Btree const &other)
    {
        _root = copy_node(other._root);
    }

    Btree(
            Btree &&other) noexcept:
            _root(std::move(other._root))
    {
        other._root = nullptr;
    }

    Btree &operator=(
            Btree const &other)
    {
        if (this != &other)
        {
            this->_root = copy_node(other._root);
        }

        return *this;
    }

    Btree &operator=(
            Btree &&other) noexcept
    {
        if (this != &other)
        {
            _root = std::move(other._root);
            other._root = nullptr;
        }

        return *this;
    }

    void find_path(
            std::stack<std::pair<B_node_ptr, int>> &path,
            T_key const &key)
    {
        if (_root == nullptr)
        {
            return;
        }

        B_node_ptr cur_node = _root;

        while (cur_node != nullptr)
        {
            auto comparison_result = 1;
            int idx;
            for (idx = 0; idx < cur_node->keys_count; ++idx)
            {
                comparison_result = _comparer(key, cur_node->element_list[idx].first);

                if (comparison_result < 0)
                {
                    break;
                }
                else if (comparison_result == 0)
                {
                    path.push(std::make_pair(cur_node, idx));

                    return;
                }
            }

            path.push(std::make_pair(cur_node, -1 - idx));
            cur_node = cur_node->child_list[idx];
        }
    }

private:

    std::optional<std::pair<std::pair<T_key, T_value>, B_node_ptr>> insert(
            B_node_ptr node,
            int index,
            std::pair<T_key, T_value> &&element,
            B_node_ptr right_subtree)
    {
        if (node->keys_count < get_max_keys_count())
        {
            for (auto i = node->keys_count; i > index; --i)
            {
                node->element_list[i] = std::move(node->element_list[i - 1]);
                node->child_list[i + 1] = node->child_list[i];
            }

            node->element_list[index] = std::move(element);
            node->child_list[index + 1] = right_subtree;
            ++node->keys_count;

            return std::nullopt;
        }

        std::vector<std::pair<T_key, T_value>> all_kvps(2 * t);
        std::vector<B_node_ptr> all_subtrees(2 * t + 1);

        for (auto i = 0; i < 2 * t; ++i)
        {
            if (i < index)
            {
                all_kvps[i] = std::move(node->element_list.at(i));
                all_subtrees[i] = node->child_list.at(i);
            }
            else if (i == index)
            {
                all_kvps[i] = std::move(element);
                all_subtrees[i] = node->child_list.at(i);
                all_subtrees[i + 1] = right_subtree;
            }
            else
            {
                all_kvps[i] = std::move(node->element_list.at(i - 1));
                all_subtrees[i + 1] = node->child_list.at(i);
            }
        }

        std::pair<T_key, T_value> median = std::move(all_kvps.at(t - 1));
        all_kvps.erase(all_kvps.cbegin() + t - 1);

        B_node_ptr new_node = std::make_shared<B_node<T_key, T_value, t>>();
        for (auto i = 0; i < t; ++i)
        {
            new_node->insert(std::move(all_kvps.at(t - 1)), i);
            all_kvps.erase(all_kvps.cbegin() + t - 1);
            new_node->child_list[i] = all_subtrees[t + i];
        }
        new_node->child_list[t] = all_subtrees[2 * t];

        node->keys_count = 0;
        for (auto i = 0; i < t - 1; ++i)
        {
            node->insert(std::move(all_kvps[i]), i);
            node->child_list[i] = all_subtrees[i];
        }
        node->child_list[t - 1] = all_subtrees[t - 1];

        return std::make_optional<std::pair<std::pair<T_key, T_value>, B_node_ptr>>(std::move(median), new_node);
    }

public:

    void insert(
            T_key const &key,
            T_value const &value)
    {
        auto new_element = std::make_pair(key, value);

        std::stack<std::pair<B_node_ptr, int>> path;

        find_path(path, key);

        // вставка в пустое дерево
        if (path.empty())
        {
            _root = std::make_shared<B_node<T_key, T_value, t>>();
            _root->insert(std::move(new_element), 0);

            return;
        }

        // вставка уже существующего ключа
        if (path.top().second >= 0)
        {
            throw Exception_Btree("retry_key_insertion");
        }

        B_node_ptr subtree = nullptr;

        while (!path.empty())
        {
            auto insertion_result = insert(path.top().first, -1 - path.top().second, std::move(new_element), subtree);
            if (!insertion_result.has_value())
            {
                return;
            }

            path.pop();
            new_element = std::move(insertion_result.value().first);
            subtree = insertion_result.value().second;
        }

        B_node_ptr new_root = std::make_shared<B_node<T_key, T_value, t>>();
        new_root->insert(std::move(new_element), 0);
        new_root->child_list[0] = _root;
        new_root->child_list[1] = subtree;
        _root = new_root;
    }

    void print(
            std::ostream &stream) const
    {
        print(_root, 0, stream);
    }

private:

    void print(
            B_node_ptr subtree_root,
            int tabs,
            std::ostream &stream) const
    {
        if (subtree_root == nullptr)
        {
            return;
        }

        for (auto i = 0; i < tabs; ++i)
        {
            stream << "  ";
        }

        for (auto i = 0; i < subtree_root->keys_count; ++i)
        {
            stream << subtree_root->element_list[i].first << ' ';
        }
        stream << std::endl;

        for (auto i = 0; i <= subtree_root->keys_count; ++i)
        {
            print(subtree_root->child_list[i], tabs + 1, stream);
        }
    }

public:

    T_value &find(
            T_key const &key)
    {
        std::stack<std::pair<B_node_ptr, int>> path;

        find_path(path, key);

        if (path.empty() || path.top().second < 0)
        {
            throw Exception_Btree("key_not_exist");
        }

        return path.top().first->element_list[path.top().second].second;
    }

    void remove(
            T_key const &key)
    {
        std::stack<std::pair<B_node_ptr, int>> path;

        find_path(path, key);

        if (path.empty() || path.top().second < 0)
        {
            throw Exception_Btree("key_not_exist");
        }

        // удаление не из терминального узла
        // сводим к случаю удаления из терминального узла
        if (path.top().first->child_list[0] != nullptr)
        {
            std::pair<B_node_ptr, int> target_path_item = path.top();
            path.pop();

            B_node_ptr iterator = target_path_item.first;
            auto index = target_path_item.second;

            while (iterator != nullptr)
            {
                path.push(std::make_pair(iterator, -1 - index));
                iterator = iterator->child_list[index];
                index = iterator == nullptr
                        ? -1
                        : iterator->keys_count;
            }

            path.top().second = -2 - path.top().second;

            auto temp = target_path_item.first->element_list[target_path_item.second];
            target_path_item.first->element_list[target_path_item.second] = path.top().first->element_list[path.top().second];
            path.top().first->element_list[path.top().second] = temp;
        }

        // удаление из терминального узла
        B_node_ptr node = path.top().first;
        node->element_list.erase(node->element_list.cbegin() + path.top().second);
        node->element_list.resize(get_max_keys_count());
        --node->keys_count;
        path.pop();

        // поднимаемся от узла, из которого удалили ключ, к корню
        while (true)
        {
            // всё хорошо, ничего не нарушено
            if (node->keys_count >= get_min_keys_count())
            {
                break;
            }

            // ключ был удалён из корня
            if (path.empty())
            {
                // нужно ли удалить корневой узел
                if (node->keys_count == 0)
                {
                    _root = node->child_list[0];
                }

                break;
            }

            B_node_ptr parent = path.top().first;
            int node_index_into_parent = -1 - path.top().second;
            path.pop();

            auto left_brother_exists = node_index_into_parent != 0;
            auto right_brother_exists = node_index_into_parent != parent->keys_count;

            auto can_take_from_left_brother = left_brother_exists
                                              && parent->child_list[node_index_into_parent - 1]->keys_count > get_min_keys_count();
            auto can_take_from_right_brother = right_brother_exists
                                               && parent->child_list[node_index_into_parent + 1]->keys_count > get_min_keys_count();

            if (can_take_from_left_brother)
            {
                auto left_brother = parent->child_list[node_index_into_parent - 1];
                auto max_from_left_brother = std::move(left_brother->element_list[left_brother->keys_count - 1]);
                auto left_brother_last_subtree = std::move(left_brother->child_list[left_brother->keys_count]);
                --left_brother->keys_count;
                auto separate = std::move(parent->element_list[node_index_into_parent - 1]);
                parent->element_list[node_index_into_parent - 1] = std::move(max_from_left_brother);
                node->element_list.insert(node->element_list.cbegin(), std::move(separate));
                node->element_list.resize(get_max_keys_count());
                node->child_list.insert(node->child_list.cbegin(), std::move(left_brother_last_subtree));
                node->child_list.resize(get_max_keys_count() + 1);
                ++node->keys_count;

                break;
            }
            else if (can_take_from_right_brother)
            {
                auto right_brother = parent->child_list[node_index_into_parent + 1];
                auto min_from_right_brother = std::move(right_brother->element_list[0]);
                right_brother->element_list.erase(right_brother->element_list.cbegin());
                right_brother->element_list.resize(get_max_keys_count());
                auto right_brother_first_subtree = std::move(right_brother->child_list[0]);
                right_brother->child_list.erase(right_brother->child_list.cbegin());
                right_brother->child_list.resize(get_max_keys_count() + 1);
                --right_brother->keys_count;
                auto separate = std::move(parent->element_list[node_index_into_parent]);
                parent->element_list[node_index_into_parent] = std::move(min_from_right_brother);
                node->element_list.insert(node->element_list.cbegin() + node->keys_count, std::move(separate));
                node->element_list.resize(get_max_keys_count());
                node->child_list.insert(node->child_list.cbegin() + node->keys_count + 1, std::move(right_brother_first_subtree));
                node->child_list.reserve(get_max_keys_count() + 1);
                ++node->keys_count;

                break;
            }

            auto separator_key_index = left_brother_exists
                                       ? node_index_into_parent - 1
                                       : node_index_into_parent;
            auto left_node_to_merge = parent->child_list[separator_key_index];
            auto right_node_to_merge = parent->child_list[separator_key_index + 1];

            left_node_to_merge->element_list.insert(left_node_to_merge->element_list.cbegin() + left_node_to_merge->keys_count, std::move(parent->element_list[separator_key_index]));
            left_node_to_merge->element_list.resize(get_max_keys_count());
            ++left_node_to_merge->keys_count;

            parent->element_list.erase(parent->element_list.cbegin() + separator_key_index);
            parent->element_list.resize(get_max_keys_count());
            --parent->keys_count;
            parent->child_list.erase(parent->child_list.cbegin() + separator_key_index + 1);
            parent->child_list.resize(get_max_keys_count() + 1);

            for (auto i = 0; i < right_node_to_merge->keys_count; ++i)
            {
                left_node_to_merge->element_list.insert(left_node_to_merge->element_list.cbegin() + left_node_to_merge->keys_count, std::move(right_node_to_merge->element_list.at(i)));
                left_node_to_merge->child_list.insert(left_node_to_merge->child_list.cbegin() + left_node_to_merge->keys_count, std::move(right_node_to_merge->child_list.at(i)));
                ++left_node_to_merge->keys_count;
            }
            left_node_to_merge->child_list.insert(left_node_to_merge->child_list.cbegin() + left_node_to_merge->keys_count, std::move(right_node_to_merge->child_list.at(right_node_to_merge->keys_count)));
            left_node_to_merge->element_list.resize(get_max_keys_count());
            left_node_to_merge->child_list.resize(get_max_keys_count() + 1);

            node = parent;
        }
    }

};

#endif //UNTITLED_BTREE_HPP
