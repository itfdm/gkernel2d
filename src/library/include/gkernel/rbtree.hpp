#ifndef __GKERNEL_HPP_RBTREE
#define __GKERNEL_HPP_RBTREE

#include <set>
#include <iterator>
#include <utility>
#include <iostream>

namespace gkernel {
    /**
     * @brief
     * Контейнер, содержащий отсортированный набор уникальных объектов типа T.
     * Сортировка осуществляется с помощью компаратора Comparator, сравнивающий значения T.
     *
     */
    template <typename T, class Comparator = std::less<T>>
    class RBTree
    {
    public:
        using iterator = typename std::set<T, Comparator>::iterator;
        enum class state
        {
            inf_negative = -1,
            exists = 0,
            inf_positive = 1
        };

        /**
         * @brief Конструктор класса.
         *
         */
        RBTree(): _internal_tree(std::less<T>{}) {}

        /**
         * @brief Конструктор класса.
         *
         * @param comp Компаратор.
         */
        RBTree(const Comparator& comp): _internal_tree(comp) {}

        /**
         * @brief Возвращает итератор на первый элемент в контейнере.
         *
         * @return auto Итератор на первый элемент в контейнере.
         */
        auto begin() const {
            return _internal_tree.begin();
        }

        /**
         * @brief Возвращает итератор на последний элемент в контейнере.
         *
         * @return auto Итератор на последний элемент в контейнере.
         */
        auto end() const {
            return _internal_tree.end();
        }

        /**
         * @brief Вставка элемента.
         *
         * @param item Вставляемый элемент.
         */
        auto insert(const T& item) {
            return _internal_tree.insert(item);
        }

        /**
         * @brief Добавляет в контейнер элементы из std::initializer_list<T> items.
         *
         * @param items Вставляемые элементы.
         */
        void insert(std::initializer_list<T> items) {
            _internal_tree.insert(items);
        }

        /**
         * @brief Возвращает максимальный элемент в контейнере.
         *
         * @return const T& максимальный элемент в контейнере.
         */
        const T& max() {
            return *(--_internal_tree.end());
        }

        /**
         * @brief Возвращает минимальный элемент в контейнере.
         *
         * @return const T& минимальный элемент в контейнере.
         */
        const T& min() {
            return *(_internal_tree.begin());
        }

        /**
         * @brief Удаляет элемент item из контейнера, если он есть.
         *
         * @param item Удаляемый элемент.
         */
        void erase(const T& item) {
            _internal_tree.erase(item);
        }

        /**
         * @brief Удаляет элемент из контейнера, на который указывает итератор.
         *
         * @param it Итератор, указывающий на удаляемый элемент.
         */
        void erase(iterator it) {
            _internal_tree.erase(it);
        }

        /**
         * @brief Возвращает количество элементов в контейнере.
         *
         * @return std::size_t количество элементов в контейнере
         */
        std::size_t size() const {
            return _internal_tree.size();
        }

        iterator find(const T& item) {
            return _internal_tree.find(item);
        }

        /**
         * @brief Возвращет следующее по величине элемент T, содержащийся в контейнере.
         *
         * @param item Элемент, с которым происходит сравнение.
         * @return std::pair<T, state> возвращает пару состоящую из следующего по величине после item значения T в RBTree и маркера state:
         *          1. если в контейнере есть значение большее, чем T, то state = exists
         *          2. иначе  state = inf_positive
         */
        std::pair<iterator, state> find_next(const T& item) {
            iterator result = _internal_tree.upper_bound(item);
            state flag = state::exists;

            if (result == _internal_tree.end()) {
                flag = state::inf_positive;
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
        std::pair<iterator, state> find_prev(const T& item) {
            iterator result = _internal_tree.lower_bound(item);
            state flag = state::exists;

            if (result == _internal_tree.begin()) {
                flag = state::inf_negative;
            }
            else {
                --result;
            }

            return std::make_pair(result, flag);
        }

    private:
        std::set<T, Comparator> _internal_tree;
    };

} // namespace gkernel
#endif /* __GKERNEL_HPP_RBTREE */
