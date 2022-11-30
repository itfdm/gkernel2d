#ifndef __GKERNEL_HPP_RBTREE
#define __GKERNEL_HPP_RBTREE

#include <set>
#include <iterator>
#include <utility>
#include <iostream>

namespace gkernel
{

    template <typename T, class Comparator = std::less<T>>
    class RBTree
    {
        /**
         * @brief
         * Контейнер, содержащий отсортированный набор уникальных объектов типа T.
         * Сортировка осуществляется с помощью компаратора Comparator, сравнивающий значения T.
         *
         */
    public:
        enum class state
        {
            inf_negative = -1,
            exists = 0,
            inf_positive = 1
        };

        /**
         * @brief Создаёт новый объект RBTree.
         *
         */
        RBTree() : _internal_tree(std::less<T>{}) {}
        /**
         * @brief Создаёт новый объект RBTree.
         *
         * @param comp компаратор
         */
        RBTree(const Comparator &comp) : _internal_tree(comp) {}

        /**
         * @brief Вставка элемента.
         *
         * @param item Вставляемый элемент.
         */
        void insert(const T &item)
        {
            _internal_tree.insert(item);
        }

        /**
         * @brief Добавляет в контейнер элементы из std::initializer_list<T> items.
         *
         * @param items Вставляемые элементы.
         */
        void insert(std::initializer_list<T> items)
        {
            _internal_tree.insert(items);
        }

        /**
         * @brief Возвращает максимальный элемент в контейнере.
         *
         * @return const T& максимальный элемент в контейнере.
         */
        const T &max()
        {
            return *(--_internal_tree.end());
        }

        /**
         * @brief Возвращает минимальный элемент в контейнере.
         *
         * @return const T& минимальный элемент в контейнере.
         */
        const T &min()
        {
            return *(_internal_tree.begin());
        }

        /**
         * @brief Удаляет элемент item из контейнера, если он есть.
         *
         * @param item Удаляемый элемент.
         */
        void erase(const T &item)
        {
            _internal_tree.erase(item);
        }

        /**
         * @brief Возвращает количество элементов в контейнере.
         *
         * @return std::size_t количество элементов в контейнере
         */
        std::size_t size() const
        {
            return _internal_tree.size();
        }

        /**
         * @brief Возвращет следующее по величине элемент T, содержащийся в контейнере.
         *
         * @param item Элемент, с которым происходит сравнение.
         * @return std::pair<T, state> возвращает пару состоящую из следующего по величине после item значения T в RBTree и маркера state:
         *          1. если в контейнере есть значение большее, чем T, то state = exists
         *          2. иначе  state = inf_positive
         */
        std::pair<T, state> find_next(const T &item)
        {
            T result;
            state flag = state::exists;

            auto item_iter = _internal_tree.upper_bound(item);

            if (item_iter == _internal_tree.end())
            {
                flag = state::inf_positive;
            }
            else
            {
                result = *item_iter;
            }

            return std::make_pair(result, flag);
        }

        /**
         * @brief Возвращет предыдущее по величине элемент T, содержащийся в контейнере.
         *
         * @param item Элемент, с которым происходит сравнение.
         * @return std::pair<T, state> std::pair<T, state> возвращает пару состоящую из предыдущего по величине относительно item значения T в RBTree и маркера state:
         *          1. если в контейнере есть значение меньшее, чем T, то state = exists
         *          2. иначе  state = inf_negative
         */
        std::pair<T, state> find_prev(const T &item)
        {
            T result;
            state flag = state::exists;

            auto item_iter = _internal_tree.lower_bound(item);

            if (item_iter == _internal_tree.begin())
            {
                flag = state::inf_negative;
            }
            else
            {
                result = *std::prev(item_iter);
            }

            return std::make_pair(result, flag);
        }

    private:
        std::set<T, Comparator> _internal_tree;
    };

} // namespace gkernel
#endif /* __GKERNEL_HPP_RBTREE */
