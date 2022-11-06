#include "gkernel/event.hpp"

namespace gkernel {
using RBtree = gkernel::RBTree<gkernel::Segment>;

gkernel::Segment pointintersection(const gkernel::Segment &a,
                                   const gkernel::Segment &b) {
    gkernel::data_type a1 = a.end_point().y() - a.begin_point().y();
    gkernel::data_type b1 = a.begin_point().x() - a.end_point().x();
    gkernel::data_type c1 = a.begin_point().y() * a.end_point().x() -
                            a.begin_point().x() * a.end_point().y();

    gkernel::data_type a2 = b.end_point().y() - b.begin_point().y();
    gkernel::data_type b2 = b.begin_point().x() - b.end_point().x();
    gkernel::data_type c2 = b.begin_point().y() * b.end_point().x() -
                            b.begin_point().x() * b.end_point().y();
    if ((a1 * b2 - a2 * b1) != 0) {
        data_type x = (data_type)(b1 * c2 - b2 * c1) / (data_type)(a1 * b2 - a2 * b1);
        data_type y = (data_type)(a2 * c1 - a1 * c2) / (data_type)(a1 * b2 - a2 * b1);
        return gkernel::Segment({x, y}, {x, y});
    } else {
        if (a.begin_point().x() <= b.begin_point().x() && b.end_point().x() <= a.end_point().x()) {

            return gkernel::Segment({b.begin_point().x(), b.begin_point().y()},
                                    {b.end_point().x(), b.end_point().y()});
        } else if (a.begin_point().x() <= b.begin_point().x() && a.end_point().x() <= b.end_point().x()) {
            return gkernel::Segment({b.begin_point().x(), b.begin_point().y()},
                                    {a.end_point().x(), a.end_point().y()});
        } else if (b.begin_point().x() <= a.begin_point().x() && a.end_point().x() <= b.end_point().x()) {
            return gkernel::Segment({a.begin_point().x(), a.begin_point().y()},
                                    {a.end_point().x(), a.end_point().y()});
        } else if (b.begin_point().x() <= a.begin_point().x() && b.end_point().x() <= a.end_point().x()) {
            return gkernel::Segment({a.begin_point().x(), a.begin_point().y()},
                                    {b.end_point().x(), b.end_point().y()});
        }
    }
}

std::set<gkernel::Segment> solve(std::vector<gkernel::Segment> &a) {
    RBtree s;
    std::vector<gkernel::Segment> where;
    std::set<gkernel::Segment> intersection;
    int n = a.size();
    std::vector<event> e;

    for (int i = 0; i < n; ++i) {
        e.push_back(event(std::min(a[i].begin_point().x(), a[i].end_point().x()), 1, i));
        e.push_back(event(std::max(a[i].begin_point().x(), a[i].end_point().x()), -1, i));
    }

    std::sort(e.begin(), e.end());
    where.resize(a.size());

    for (size_t i = 0; i < e.size(); ++i) {
        int id = e[i].id;
        if (e[i].tp == 1) {
            auto nxt = s.find_next(a[id]);
            auto prv = s.find_prev(nxt.first);

            if (nxt.second == RBtree::state::exists)
                if (intersect(nxt.first, a[id])) {
                    intersection.insert(pointintersection(nxt.first, a[id]));
                }
                else {
                    if (intersect_parallel(nxt.first, a[id])) {
                        intersection.insert(pointintersection(nxt.first, a[id]));
                    }
                }

            if (prv.second == RBtree::state::exists && intersect(prv.first, a[id])) {
                if (intersect(prv.first, a[id])) {
                    intersection.insert(pointintersection(prv.first, a[id]));
                }
                else {
                    if (intersect_parallel(prv.first, a[id])) {
                        intersection.insert(pointintersection(prv.first, a[id]));
                    }
                }
            }
            s.insert(a[id]);
            where[id] = a[id];
        } else {
            auto nxt = s.find_next(where[id]);
            auto prv = s.find_prev(nxt.first);
            if (nxt.second == RBtree::state::exists && prv.second == RBtree::state::exists) {
                if (intersect(nxt.first, prv.first)) {
                    intersection.insert(pointintersection(nxt.first, prv.first));
                }
                else {
                    intersection.insert(pointintersection(nxt.first, prv.first));
                }
            }
            s.erase(where[id]);
        }
    }
    return intersection;
}

} // namespace gkernel
