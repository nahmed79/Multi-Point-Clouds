#include <cmath>
#include <limits>

#include "plane.h"

bool plane::intersect(const ray& r, float& t) const
{
	float nc = norm * r.dir;

	if ( fabs(nc) <= 1e-12 )
		return false;

	float nba = norm * (root - r.start);

	float t0 = nba / nc;

	if ( t0 <= t )
	{
		t = t0;
		return true;
	}

	return false;  
}