#ifndef __DEF_H
#define __DEF_H

#include <cmath>

const double PIOVER180 = 0.017453292519943295769236907684886;

struct Point {
	float x, y, z;
};

struct Vector {
	float x, y, z;

    Vector& operator += (const Vector &v2){
	    this->x += v2.x;
        this->y += v2.y;
        this->z += v2.z;
	    return *this;
    }

	inline float norm() const {
		return sqrtf( x * x + y * y + z * z);
	}

	inline bool normalize() {
		float n = norm();

		if( fabs(n) <= 1e-12 )
			return false;

		x /= n;
		y /= n;
		z /= n;

		return true;
	}

};

inline Point operator + (const Point&p, const Vector &v){
	Point p2={p.x + v.x, p.y + v.y, p.z + v.z };
	return p2;
}

inline Point operator - (const Point&p, const Vector &v){
	Point p2={p.x - v.x, p.y - v.y, p.z - v.z };
	return p2;
}

inline Vector operator + (const Vector&v1, const Vector &v2){
	Vector v={v1.x + v2.x, v1.y + v2.y, v1.z + v2.z };
	return v;
}

inline Vector operator - (const Point&p1, const Point &p2){
	Vector v={p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
	return v;
}

inline Vector operator * (float c, const Vector &v)
{
	Vector v2={v.x *c, v.y * c, v.z * c };
	return v2;
}

inline Vector operator * (const Vector &v, float c)
{
	Vector v2={v.x *c, v.y * c, v.z * c };
	return v2;
}

inline Vector operator - (const Vector&v1, const Vector &v2){
	Vector v={v1.x - v2.x, v1.y - v2.y, v1.z - v2.z };
	return v;
}

inline float operator * (const Vector&v1, const Vector &v2 ) {
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline Vector operator % (const Vector&v1, const Vector &v2 ) {
	Vector v;
	v.x = v1.y * v2.z - v1.z * v2.y;
	v.y = v1.z * v2.x - v1.x * v2.z;
	v.z = v1.x * v2.y - v1.y * v2.x;
	return v;
}

struct color {
    enum OFFSET 
    {
        OFFSET_RED = 0,
        OFFSET_GREEN = 1,
        OFFSET_BLUE = 2,
        OFFSET_MAX  = 3
    };
    float red, green, blue;

    inline color & operator += (const color &c2 ) {
	    this->red +=  c2.red;
        this->green += c2.green;
        this->blue += c2.blue;
	    return *this;
    }

    inline float & getChannel(OFFSET offset )
    {
        return reinterpret_cast<float*>(this)[offset];
    }

    inline float getChannel(OFFSET offset ) const
    {
        return reinterpret_cast<const float*>(this)[offset];
    }
};

inline color operator * (const color&c1, const color &c2 ) {
	color c = {c1.red * c2.red, c1.green * c2.green, c1.blue * c2.blue};
	return c;
}

inline color operator + (const color&c1, const color &c2 ) {
	color c = {c1.red + c2.red, c1.green + c2.green, c1.blue + c2.blue};
	return c;
}

inline color operator * (float coef, const color &c ) {
	color c2 = {c.red * coef, c.green * coef, c.blue * coef};
	return c2;
}

#endif //__DEF_H
