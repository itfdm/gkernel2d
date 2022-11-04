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
    enum class state {
        inf_negative = -1,
        exists = 0,
        inf_positive = 1
    };

    RBTree() : _internal_tree(std::less<T>{}) { }
    RBTree(const Comparator& comp) : _internal_tree(comp) { }

    void insert(const T& item) {
        _internal_tree.insert(item);
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

    void erase(const T& item) {
        _internal_tree.erase(item);
    }

    std::size_t size() const {
        return _internal_tree.size();
    }

    std::pair<T, state> find_next(const T& item) {
        T result;
        state flag = state::exists;

        auto item_iter = _internal_tree.upper_bound(item);

        if (item_iter == _internal_tree.end()) {
            flag = state::inf_positive;
        } else {
            result = *item_iter;
        }

        return std::make_pair(result, flag);
    }

    std::pair<T, state> find_prev(const T& item) {
        T result;
        state flag = state::exists;

        auto item_iter = _internal_tree.lower_bound(item);

        if (item_iter == _internal_tree.begin()) {
            flag = state::inf_negative;
        } else {
            result = *std::prev(item_iter);
        }

        return std::make_pair(result, flag);
    }

private:
    std::set<T, Comparator> _internal_tree;
};

}  // namespace gkernel
#endif /* __GKERNEL_HPP_RBTREE */
