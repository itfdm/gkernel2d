#ifndef __GKERNEL_HPP_RBTREE
#define __GKERNEL_HPP_RBTREE

#include <set>
#include <vector>
#include <iterator>
#include <utility>
#include <iostream>

namespace gkernel {

template<typename T, class Comparator = std::less<T>>
class RBTree {
public:
    using iterator = typename std::set<T, Comparator>::iterator;
    using node_type = typename std::set<T, Comparator>::node_type;
    enum class state {
        inf_negative = -1,
        exists = 0,
        inf_positive = 1
    };

    RBTree() : _internal_tree(std::less<T>{}) { }
    RBTree(const Comparator& comp) : _internal_tree(comp) { }

    // begin iterator for _internal_tree
    auto begin() const {
        return _internal_tree.begin();
    }

    // end iterator for _internal_tree
    auto end() const {
        return _internal_tree.end();
    }

    auto insert(const T& item) {
        return _internal_tree.insert(item);
    }

    void insert(std::initializer_list<T> items) {
        _internal_tree.insert(items);
    }

    void insert(node_type&& node) {
        _internal_tree.insert(std::move(node));
    }

    const T& max() {
        return *(--_internal_tree.end());
    }

    const T& min() {
        return *(_internal_tree.begin());
    }

    auto erase(const T& item) {
        return _internal_tree.erase(item);
    }

    auto erase(const iterator& it) {
        return _internal_tree.erase(it);
    }

    std::size_t size() const {
        return _internal_tree.size();
    }

    auto find(const T& item) const {
        return _internal_tree.find(item);
    }

    auto extract(const T& item) {
        return _internal_tree.extract(item);
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
