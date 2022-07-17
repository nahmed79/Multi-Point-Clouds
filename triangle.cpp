#include <cmath>

#include "triangle.h"

bool triangle::intersect(const ray& r, float& t) const
{
	float t0 = t;

	if ( !pl.intersect(r, t0) )
		return false;

	Point pHit = r.start + r.dir * t0;

	Vector v0 = p[1] - p[0];
	Vector v1 = p[2] - p[0];
	Vector v2 = pHit - p[0];

	float dot00 = v0 * v0;
	float dot01 = v0 * v1;
	float dot02 = v0 * v2;
	float dot11 = v1 * v1;
	float dot12 = v1 * v2;

	// Compute barycentric coordinates
	float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
	float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

	// Check if point is in triangle
	if ( (u >= -1e-7) && (v >= -1e-7) && (u + v <= 1.0) )
	{
		t = t0;
		return true;
	}

	return false;
}