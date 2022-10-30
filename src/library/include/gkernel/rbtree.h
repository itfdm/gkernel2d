#ifndef __GKERNEL_H_RBTREE
#define __GKERNEL_H_RBTREE

#include <set>
#include <iterator>
#include <utility>
#include <iostream>

namespace gkernel {

template<typename T, class Comparator = std::less<T>>
class RBTree {
public:
    enum class inf_state {
        is_negative = -1,
        is_number = 0,
        is_positive = 1
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

    std::pair<T, inf_state> find_next(const T& item) {
        T result;
        inf_state flag = inf_state::is_number;

        auto item_iter = _internal_tree.insert(item);
        auto next_item_iter = std::next(item_iter.first);

        if (next_item_iter == _internal_tree.end()) {
            flag = inf_state::is_positive;
        } else {
            result = *next_item_iter;
        }

        if (item_iter.second) {
            _internal_tree.erase(item_iter.first);
        }

        return std::make_pair(result, flag);
    }

    std::pair<T, inf_state> find_prev(const T& item) {
        T result;
        inf_state flag = inf_state::is_number;

        auto item_iter = _internal_tree.insert(item);

        if (item_iter.first == _internal_tree.begin()) {
            flag = inf_state::is_negative;
        } else {
            result = *std::prev(item_iter.first);
        }

        if (item_iter.second) {
            _internal_tree.erase(item_iter.first);
        }

        return std::make_pair(result, flag);
    }

private:
    std::set<T, Comparator> _internal_tree;
};

}  // namespace gkernel
#endif /* __GKERNEL_H_RBTREE */
