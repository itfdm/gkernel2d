#ifndef __GKERNEL_HPP_EVENT
#define __GKERNEL_HPP_EVENT

#include "containers.hpp"
#include "objects.hpp"
#include "rbtree.hpp"

namespace gkernel {

const double EPS = 1E-9;

inline bool intersect1d(gkernel::data_type l1, gkernel::data_type r1,
                        gkernel::data_type l2, gkernel::data_type r2) {
    if (l1 > r1) std::swap(l1, r1);
    if (l2 > r2) std::swap(l2, r2);
    return std::max(l1, l2) <= std::min(r1, r2) + EPS;
}

inline int vec(const gkernel::Point &a, const gkernel::Point &b,
               const gkernel::Point &c) {
    gkernel::data_type s = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
    return abs(s) < EPS ? 0 : s > 0 ? 1 : -1;
}

bool intersect(const gkernel::Segment &a, const gkernel::Segment &b) {
    return intersect1d(a.begin_point().x(), a.end_point().x(), b.begin_point().x(), b.end_point().x()) &&
           intersect1d(a.begin_point().y(), a.end_point().y(), b.begin_point().y(), b.end_point().y()) &&
           vec(a.begin_point(), a.end_point(), b.begin_point()) * vec(a.begin_point(), a.end_point(), b.end_point()) <= 0 &&
           vec(b.begin_point(), b.end_point(), a.begin_point()) * vec(b.begin_point(), b.end_point(), a.end_point()) <= 0;
}

struct event {
	Segment seg;
    gkernel::data_type x, y;
    int tp, id;

    event() {}
    event(gkernel::Segment seg, gkernel::data_type x, gkernel::data_type y, int tp, int id) : seg(seg), x(x), y(y), tp(tp), id(id) {}

    bool operator<(const event &e) const {
		if (abs(x - e.x) > EPS)  
            return x < e.x;
        else if (abs(y - e.y) > EPS) return y < e.y;
        else if (abs(e.tp - tp) > EPS) return tp > e.tp;
        else return id < e.id;
    }
};

} // namespace gkernel
#endif // __GKERNEL_HPP_EVENT
