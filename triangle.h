#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "plane.h"

struct triangle {
	Point p[3];
	plane pl;

	bool intersect(const ray& r, float& t) const;

	inline triangle(const Point &_p1, const Point &_p2, const Point &_p3) {
		init(_p1, _p2, _p3);
	}

	inline triangle() {}

	void compute_normal();

	inline void init(const Point &_p1, const Point &_p2, const Point &_p3) {
		p[0] = _p1;
		p[1] = _p2;
		p[2] = _p3;

		Vector a = p[1] - p[0];
		Vector b = p[2] - p[0];

		Vector norm = b % a;
		norm = a % b;
		norm.normalize();

		pl.root = p[0];
		pl.norm = norm;			
	}
};

#endif