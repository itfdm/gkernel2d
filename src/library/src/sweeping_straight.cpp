#include "gkernel/event.hpp"
#include "gkernel/containers.hpp"
#include <functional>

namespace gkernel {
using RBtree = gkernel::RBTree<gkernel::Segment, std::function<bool(const Segment& c1, const Segment& c2)>>;

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

data_type Y(const gkernel::Segment &seg, data_type x) {
            double alfa = (seg.end_point().x() - x - EPS) / (seg.end_point().x() - seg.begin_point().x());
            return alfa * seg.begin_point().y() + (1 - alfa) * seg.end_point().y();
}

RBtree solve(gkernel::SegmentsSet &a) {


    std::set<event> e;

    int count = a.size();
    for (int i = 0; i < a.size(); ++i) {
        if (a[i].begin_point().x() == std::min(a[i].begin_point().x(), a[i].end_point().x())) {
            e.insert(event(Segment({ a[i].begin_point().x(), a[i].begin_point().y() }, { a[i].end_point().x(), a[i].end_point().y() }), a[i].begin_point().x(), a[i].begin_point().y(), 1, i));
            e.insert(event(Segment({ a[i].begin_point().x(), a[i].begin_point().y() }, { a[i].end_point().x(), a[i].end_point().y() }), a[i].end_point().x(), a[i].end_point().y(), -1, i));
        }
        else {
            e.insert(event(Segment({ a[i].begin_point().x(), a[i].begin_point().y() }, { a[i].end_point().x(), a[i].end_point().y() }), a[i].end_point().x(), a[i].end_point().y(), 1, i));
            e.insert(event(Segment({ a[i].begin_point().x(), a[i].begin_point().y() }, { a[i].end_point().x(), a[i].end_point().y() }), a[i].begin_point().x(), a[i].begin_point().y(), -1, i));
        }
    }

    data_type x;
    auto comparator = [&x, &a](const Segment& c1, const Segment& c2) -> bool {
         double res = Y(c1, x) - Y(c2, x);
         if (res != 0) return Y(c1, x) < Y(c2, x);
         return c1.id < c2.id;
    };
    auto inter_comparator = [&x, &a](const Segment& c1, const Segment& c2) -> bool {
        if (c1.begin_point().x() < c2.begin_point().x()) {
            return true;
        }
        else if (c1.begin_point().x() == c2.begin_point().x()) {
            if (c1.begin_point().y() < c2.begin_point().y()) {
                return true;
            }
            else if (c1.begin_point().y() == c2.begin_point().y()) {
                if (c1.end_point().x() < c2.end_point().x()) {
                    return true;
                }
                else if (c1.end_point().x() == c2.end_point().x()) {
                    if (c1.end_point().x() < c2.end_point().x()) {
                        return true;
                    }
                }
            }
        }
        return false;
    };
    RBtree s(comparator);
    RBtree intersection(inter_comparator);
    while (e.size()) {
        x = (*e.begin()).x;
        if ((*e.begin()).seg.begin_point().x() == (*e.begin()).seg.end_point().x() && (*e.begin()).tp == 1) {

        }
        else if ((*e.begin()).tp == 1) {
            auto nxt = s.find_next((*e.begin()).seg);
            auto prv = s.find_prev((*e.begin()).seg);
            auto ie = (*e.begin()).seg;
            if (nxt.second == RBtree::state::exists)
                if (intersect(nxt.first, (*e.begin()).seg)) {
                    Segment buf = pointintersection(nxt.first, (*e.begin()).seg);
                    if (!intersection.contains(buf)) {
                        intersection.insert(buf);
                        s.erase(nxt.first);
                        if (buf.end_point().x() != nxt.first.end_point().x() && buf.end_point().y() != nxt.first.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { nxt.first.end_point().x(), nxt.first.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                        if (buf.end_point().x() != (*e.begin()).seg.end_point().x() && buf.end_point().y() != (*e.begin()).seg.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { (*e.begin()).seg.end_point().x(), (*e.begin()).seg.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                    }
                }
                

            if (prv.second == RBtree::state::exists) {
                if (intersect(prv.first, (*e.begin()).seg)) {
                    Segment buf = pointintersection(prv.first, (*e.begin()).seg);
                    if (!intersection.contains(buf)) {
                        intersection.insert(buf);
                        s.erase(prv.first);
                        if (buf.end_point().x() != prv.first.end_point().x() && buf.end_point().y() != prv.first.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { prv.first.end_point().x(), prv.first.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                        if (buf.end_point().x() != (*e.begin()).seg.end_point().x() && buf.end_point().y() != (*e.begin()).seg.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { (*e.begin()).seg.end_point().x(), (*e.begin()).seg.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                        }
                }
            }
            s.insert((*e.begin()).seg);
            e.erase(e.begin());
        } else {
            auto nxt = s.find_next((*e.begin()).seg);
            auto prv = s.find_prev((*e.begin()).seg);
            if (nxt.second == RBtree::state::exists && prv.second == RBtree::state::exists) {
                if (intersect(nxt.first, prv.first))
                {
                    Segment buf = pointintersection(nxt.first, prv.first);
                    if (!intersection.contains(buf)) {
                        intersection.insert(buf);
                        s.erase(nxt.first);
                        s.erase(prv.first);
                        if (buf.end_point().x() != prv.first.end_point().x() && buf.end_point().y() != prv.first.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { prv.first.end_point().x(), prv.first.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                        if (buf.end_point().x() != nxt.first.end_point().x() && buf.end_point().y() != nxt.first.end_point().y())
                            e.insert(event(Segment({ buf.begin_point().x(), buf.begin_point().y() }, { nxt.first.end_point().x(), nxt.first.end_point().y() }), buf.begin_point().x(), buf.begin_point().y(), 1, count++));
                    }
               }
            }
            s.erase((*e.begin()).seg);
            e.erase(e.begin());
        }
    }
    return intersection;
}

} // namespace gkernel
