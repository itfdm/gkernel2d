#ifndef __GKERNEL_HPP_RBTREE
#define __GKERNEL_HPP_RBTREE

#include <set>
#include <iterator>
#include <utility>
#include <iostream>

namespace gkernel {

template<typename T, class Comparator = std::less<T>>
class RBTree {
public:
    using iterator = typename std::set<T, Comparator>::iterator;
    enum class state {
        inf_negative = -1,
        exists = 0,
        inf_positive = 1
    };

    RBTree() : _internal_tree(std::less<T>{}) { }
    RBTree(const Comparator& comp) : _internal_tree(comp) { }

    auto begin() const {
        return _internal_tree.begin();
    }

    auto end() const {
        return _internal_tree.end();
    }

    auto insert(const T& item) {
        return _internal_tree.insert(item);
    }

    void insert(std::initializer_list<T> items) {
        _internal_tree.insert(items);
    }

    const T& max() {
        return *(--_internal_tree.end());
    }

    const T& min() {
        return *(_internal_tree.begin());
    }

    std::size_t erase(const T& item) {
        return _internal_tree.erase(item);
    }

    void erase(iterator it) {
        _internal_tree.erase(it);
    }

    std::size_t size() const {
        return _internal_tree.size();
    }

    iterator find(const T& item) {
        return _internal_tree.find(item);
    }

    std::pair<iterator, state> find_next(const T& item) {
        iterator result = _internal_tree.upper_bound(item);
        state flag = state::exists;

        if (result == _internal_tree.end()) {
            flag = state::inf_positive;
        }

        return std::make_pair(result, flag);
    }

    std::pair<iterator, state> find_prev(const T& item) {
        iterator result = _internal_tree.lower_bound(item);
        state flag = state::exists;

        if (result == _internal_tree.begin()) {
            flag = state::inf_negative;
        } else {
            --result;
        }

        return std::make_pair(result, flag);
    }

private:
    std::set<T, Comparator> _internal_tree;
};

}  // namespace gkernel
#endif /* __GKERNEL_HPP_RBTREE */
