#ifndef __GKERNEL_HPP_EVENT
#define __GKERNEL_HPP_EVENT

#include "objects.hpp"
#include "containers.hpp"
#include "rbtree.hpp"

namespace gkernel {

const double EPS = 1E-9;

inline bool intersect1d(gkernel::data_type l1, gkernel::data_type r1, gkernel::data_type l2, gkernel::data_type r2) {
	if (l1 > r1)  std::swap(l1, r1);
	if (l2 > r2)  std::swap (l2, r2);
	return std::max (l1, l2) <= std::min (r1, r2) + EPS;
}

inline int vec(const gkernel::Point &a, const gkernel::Point &b, const gkernel::Point &c) {
	gkernel::data_type s = (b.x() - a.x()) * (c.y() - a.y()) - (b.y() - a.y()) * (c.x() - a.x());
	return abs(s)<EPS ? 0 : s>0 ? 1 : -1;
}

bool intersect (const gkernel::Segment &a, const gkernel::Segment &b) {
	return intersect1d(a.begin_point().x(), a.end_point().x(), b.begin_point().x(), b.end_point().x())
		&& intersect1d(a.begin_point().y(), a.end_point().y(), b.begin_point().y(), b.end_point().y())
		&& vec(a.begin_point(), a.end_point(), b.begin_point()) * vec(a.begin_point(), a.end_point(), b.end_point()) <= 0
		&& vec(b.begin_point(), b.end_point(), a.begin_point()) * vec(b.begin_point(), b.end_point(), a.end_point()) <= 0;
}

inline int det (int a, int b, int c, int d) {
	return a * d - b * c;
}

inline bool between (int a, int b, double c) {
	return std::min(a,b) <= c + EPS && c <= std::max(a,b) + EPS;
}

inline bool intersect_1 (int a, int b, int c, int d) {
	if (a > b)  std::swap (a, b);
	if (c > d)  std::swap (c, d);
	return std::max(a,c) <= std::min(b,d);
}

bool intersect_parallel (gkernel::Segment &a, gkernel::Segment &b) {
	int A1 = a.begin_point().y()-a.end_point().y(),  B1 = a.end_point().x()-a.begin_point().x(),  C1 = -A1*a.begin_point().x() - B1*a.begin_point().y();
	int A2 = b.begin_point().y()-b.end_point().y(),  B2 = b.end_point().x()-b.begin_point().x(),  C2 = -A2*b.begin_point().x() - B2*b.begin_point().y();
	int zn = det (A1, B1, A2, B2);
	if (zn != 0) {
		double x = - det (C1, B1, C2, B2) * 1. / zn;
		double y = - det (A1, C1, A2, C2) * 1. / zn;
		return between (a.begin_point().x(), a.end_point().x(), x) && between (a.begin_point().y(), a.end_point().y(), y)
			&& between (b.begin_point().x(), b.end_point().x(), x) && between (b.begin_point().y(), b.end_point().y(), y);
	}
	else
		return det (A1, C1, A2, C2) == 0 && det (B1, C1, B2, C2) == 0
			&& intersect_1 (a.begin_point().x(), a.end_point().x(), b.begin_point().x(), b.end_point().x())
			&& intersect_1 (a.begin_point().y(), a.end_point().y(), b.begin_point().y(), b.end_point().y());
}

struct event {
	gkernel::data_type x;
	int tp, id;

	event() { }
	event(gkernel::data_type x, int tp, int id)
		: x(x), tp(tp), id(id)
	{ }

	bool operator<(const event & e) const {
		if (abs(x - e.x) > EPS)  return x < e.x;
		return tp > e.tp;
	}
};

} // namespace gkernel
#endif // __GKERNEL_HPP_EVENT
