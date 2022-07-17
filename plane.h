#ifndef PLANE_H
#define PLANE_H

#include "main.h"

struct plane {
	Point root;
	Vector norm;

	bool intersect(const ray& r, float& t) const;
};

#endif