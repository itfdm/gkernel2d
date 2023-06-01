#ifndef __GKERNEL_HPP_RBTREE
#define __GKERNEL_HPP_RBTREE

#include <set>
#include <iterator>
#include <utility>
#include <iostream>
#include <tuple>
#include <vector>
#include "gkernel/objects.hpp"
#include "gkernel/intersection.hpp"

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

class ActiveSegmentsComparator {
public:
    ActiveSegmentsComparator(double& x_sweeping_line, const std::vector<Segment>& segments, const std::vector<std::size_t>& segment_indices) :
        _x_sweeping_line(x_sweeping_line),
        _segments(segments),
        _segment_indices(segment_indices) {}
    bool operator()(std::size_t lhs, std::size_t rhs) const {
        double eps = -EPS;
        double y1 = get_sweeping_line_y(_segments[_segment_indices[lhs]], this->_x_sweeping_line, eps);
        double y2 = get_sweeping_line_y(_segments[_segment_indices[rhs]], this->_x_sweeping_line, eps);
        auto first_idx = _segments[_segment_indices[lhs]].get_id();
        auto second_idx = _segments[_segment_indices[rhs]].get_id();

        return std::tie(y1, first_idx) < std::tie(y2, second_idx);
    }
private:
    double& _x_sweeping_line;
    const std::vector<Segment>& _segments;
    const std::vector<std::size_t>& _segment_indices;
};

class ActiveSegmentsTree {
public:
    using tree_t = RBTree<std::size_t, ActiveSegmentsComparator>;
    ActiveSegmentsTree(double& x_sweeping_line, const std::vector<Segment>& segments) : _segments(segments), _x_sweeping_line(x_sweeping_line), _comparator(x_sweeping_line, segments, _segment_indices) {
        for (std::size_t idx = 0; idx < segments.size(); ++idx) {
            _segment_indices.push_back(idx);
            _reversed_segment_indices.push_back(idx);
            segment_indices_bak.push_back(idx);
        }
        _tree = new tree_t(_comparator);
        old_segment_indices.reserve(_segment_indices.size());
        new_segment_indices.reserve(_segment_indices.size());
    }

    class ActiveSegmentsTreeIterator {
    public:
        ActiveSegmentsTreeIterator(const std::vector<Segment>& segments, const std::vector<std::size_t>& segment_indices, const tree_t::iterator& iter) :
            _segments(segments), _segment_indices(segment_indices), _iter(iter) { }

        ActiveSegmentsTreeIterator& operator++() {
            ++_iter;
            return *this;
        }

        ActiveSegmentsTreeIterator& operator--() {
            --_iter;
            return *this;
        }

        const Segment& operator*() const {
            return _segments[_segment_indices[*_iter]];
        }

        bool operator==(const ActiveSegmentsTreeIterator& other) const {
            return _iter == other._iter;
        }

        bool operator!=(const ActiveSegmentsTreeIterator& other) const {
            return _iter != other._iter;
        }

    private:
        const std::vector<Segment>& _segments;
        const std::vector<std::size_t>& _segment_indices;
        tree_t::iterator _iter;
        friend class ActiveSegmentsTree;
    };

    void reorder(std::vector<const Segment*> segments, double new_x_sweeping_line) {
        old_segment_indices.clear();
        new_segment_indices.clear();

        for (std::size_t idx = 0; idx < segments.size(); ++idx) {
            old_segment_indices.push_back(_reversed_segment_indices[segments[idx]->get_id()]);
            new_segment_indices.push_back(_reversed_segment_indices[segments[idx]->get_id()]);
        }
        std::sort(old_segment_indices.begin(), old_segment_indices.end(), _comparator);
        _x_sweeping_line = new_x_sweeping_line;
        std::sort(new_segment_indices.begin(), new_segment_indices.end(), _comparator);
        for (std::size_t idx = 0; idx < segments.size(); ++idx) {
            _segment_indices[old_segment_indices[idx]] = segment_indices_bak[new_segment_indices[idx]];
            _reversed_segment_indices[segment_indices_bak[new_segment_indices[idx]]] = old_segment_indices[idx];
        }
        for (std::size_t idx = 0; idx < segments.size(); ++idx) {
            segment_indices_bak[old_segment_indices[idx]] = _segment_indices[old_segment_indices[idx]];
        }
    }

    auto begin() const {
        return ActiveSegmentsTreeIterator(_segments, _segment_indices, _tree->begin());
    }

    auto end() const {
        return ActiveSegmentsTreeIterator(_segments, _segment_indices, _tree->end());
    }

    auto insert(const Segment& item) {
        auto insert_result = _tree->insert(_reversed_segment_indices[item.get_id()]);
        return std::make_pair(ActiveSegmentsTreeIterator(_segments, _segment_indices, insert_result.first), insert_result.second);
    }

    const Segment& max() {
        return _segments[_segment_indices[_tree->max()]];
    }

    const Segment& min() {
        return _segments[_segment_indices[_tree->min()]];
    }

    std::size_t erase(const Segment& item) {
        return _tree->erase(_reversed_segment_indices[item.get_id()]);
    }

    void erase(ActiveSegmentsTreeIterator it) {
        _tree->erase(it._iter);
    }

    std::size_t size() const {
        return _tree->size();
    }

    ActiveSegmentsTreeIterator find(const Segment& item) {
        return ActiveSegmentsTreeIterator(_segments, _segment_indices, _tree->find(_segment_indices[item.get_id()]));
    }

    std::pair<ActiveSegmentsTreeIterator, tree_t::state> find_next(const Segment& item) {
        auto found = _tree->find_next(_segment_indices[item.get_id()]);
        return std::make_pair(ActiveSegmentsTreeIterator(_segments, _segment_indices, found.first), found.second);
    }

    std::pair<ActiveSegmentsTreeIterator, tree_t::state> find_prev(const Segment& item) {
        const_cast<std::vector<Segment>&>(_segments)[_segments.size() - 1] = item;
        auto found = _tree->find_prev(_segments.size() - 1);
        return std::make_pair(ActiveSegmentsTreeIterator(_segments, _segment_indices, found.first), found.second);
    }

private:
    const std::vector<Segment>& _segments;
    double& _x_sweeping_line;
    RBTree<std::size_t, ActiveSegmentsComparator>* _tree;
    ActiveSegmentsComparator _comparator;
    std::vector<std::size_t> _segment_indices;
    std::vector<std::size_t> _reversed_segment_indices;
    std::vector<std::size_t> old_segment_indices;
    std::vector<std::size_t> new_segment_indices;
    std::vector<std::size_t> segment_indices_bak;
};

}  // namespace gkernel
#endif /* __GKERNEL_HPP_RBTREE */
