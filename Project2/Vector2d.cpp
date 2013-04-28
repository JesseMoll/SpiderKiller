//----------------------------------------------------------------------
//	Best if viewed with tabs set every 4 columns.
//----------------------------------------------------------------------
//	File:			Vector2d.cpp
//	Description:	2-dimensional point
///	Programmer:		Dave Mount
//	For:			CMSC 425 - Game Programming
//	Date:			Spring 2013
//----------------------------------------------------------------------

#include "Vector2d.h"
#include <algorithm>

//----------------------------------------------------------------------
// Vector2d methods
//----------------------------------------------------------------------
											// constructors
Vector2d::Vector2d() : x(0), y(0) { }

Vector2d::Vector2d(Scalar _x, Scalar _y) : x(_x), y(_y) { }

Vector2d::Vector2d(const Vector2d& v) : x(v.x), y(v.y) { }


Vector3d::Vector3d() : x(0), y(0), z(0) { }
Vector3d::Vector3d(Scalar _x, Scalar _y, Scalar _z) : x(_x), y(_y), z(_z) { }
Vector3d::Vector3d(const Vector3d& v) : x(v.x), y(v.y), z(v.z) { }

// assignment ops
Vector2d& Vector2d::operator=(const Vector2d& v) 
	{ x = v.x; y = v.y; return *this; }

Vector2d& Vector2d::operator+=(const Vector2d& v)
	{ x += v.x; y += v.y; return *this; }

Vector2d& Vector2d::operator-=(const Vector2d& v) 
	{ x -= v.x; y -= v.y; return *this; }


Vector2d& Vector2d::operator*=(Scalar s) 
	{ x *= s; y *= s; return *this; }

Vector2d& Vector2d::operator/=(Scalar s) 
	{ x /= s; y /= s; return *this; }
											// arithmetic ops
Vector2d Vector2d::operator+(const Vector2d& v) const 
	{ return Vector2d(x + v.x, y + v.y); }

Vector2d Vector2d::operator*(const Vector2d& v) const //item by item Multiplication (not dot or cross product)
	{ return Vector2d(x * v.x, y * v.y); }

Vector2d Vector2d::operator/(const Vector2d& v) const //item by item Division
	{ return Vector2d(x / v.x, y / v.y); }

Vector2d Vector2d::operator-(const Vector2d& v) const 
	{ return Vector2d(x - v.x, y -v.y); }

Vector2d Vector2d::operator*(Scalar s) const
	{ return Vector2d(s*x, s*y); }

Vector2d Vector2d::operator/(Scalar s) const
	{ return Vector2d(x/s, y/s); }

Vector2d Vector2d::operator-() const
	{ return Vector2d(-x, -y); }

Scalar Vector2d::length() const
	{ return sqrt(x*x + y*y); }

Scalar Vector2d::length_squared() const
	{ return (x*x + y*y); }


void Vector2d::normalize() {				// normalize to unit length
	Scalar w = length(); 
	if (w == 0) { x = 1; y = 0; }			// too short!
	else { x /= w; y /= w; }				// scale by 1/length
}

Vector2d Vector2d::exp(const Vector2d& v)
{
	Vector2d tmp; 
	tmp.x = std::exp(v.x); 
	tmp.y = std::exp(v.y); 
	return tmp;
}

Vector2d Vector2d::normalize(const Vector2d& v) {
	Vector2d tmp = v; 
	tmp.normalize(); 
	return tmp;
}

											// dot product
Scalar Vector2d::dot(const Vector2d& v, const Vector2d& w)
	{ return v.x * w.x + v.y * w.y; }

											// distance
Scalar Vector2d::distance(const Vector2d& v, const Vector2d& w)
	{ return (v - w).length(); }

Scalar Vector2d::distance_squared(const Vector2d& v, const Vector2d& w)
	{ return (v - w).length_squared(); }

											// project v parallel to w
Vector2d Vector2d::parProject(const Vector2d& v, const Vector2d& w)
	{ return w * (dot(v,w) / dot(w,w)); }

											// project v orthogonal to w
Vector2d Vector2d::orthProject(const Vector2d& v, const Vector2d& w)
	{ return v - parProject(v, w); } 

											// output operator
std::ostream& operator<<(std::ostream& out, const Vector2d& p) 
{
	out << "(" << p.x << ", " << p.y << ")";
	return out;
}

//----------------------------------------------------------------------
// Rect2d methods
//----------------------------------------------------------------------

											// constructors
Rect2d::Rect2d() : lowLeft(0,0), upRight(1,1) { }

Rect2d::Rect2d(const Point2d& l, const Point2d& u) : lowLeft(l), upRight(u) { }

Point2d Rect2d::getLowerLeft() const			// get corners
	{ return lowLeft; }

Point2d Rect2d::getUpperRight() const
	{ return upRight; }

bool Rect2d::contains(const Point2d& p) const // do we contain p?
{
	return	lowLeft.x <= p.x &&
			p.x <= upRight.x &&
			lowLeft.y <= p.y &&
			p.y <= upRight.y;
}

bool Rect2d::overlaps(const Rect2d& r) const // do we overlap r?
{
	return !(lowLeft.x > r.upRight.x ||
			 lowLeft.y > r.upRight.y ||
			 r.lowLeft.x > upRight.x ||
			 r.lowLeft.y > upRight.y);
}
	Rect2d& Rect2d::operator+=(const Vector2d& v)
	{
		upRight += v;
		lowLeft += v;
		return *this;
	}
											// translate r by t
Rect2d translate(const Rect2d& r, const Vector2d& t)
{
	Point2d ll = r.getLowerLeft();
	Point2d ur = r.getUpperRight();
	return Rect2d(Point2d(ll.x + t.x, ll.y + t.y),
				  Point2d(ur.x + t.x, ur.y + t.y));
}

											// merge two rectangles into a larger one which bounds both
Rect2d mergeBounds(const Rect2d& r1, const Rect2d& r2)
{
	Point2d ll1 = r1.getLowerLeft();
	Point2d ur1 = r1.getUpperRight();

	Point2d ll2 = r2.getLowerLeft();
	Point2d ur2 = r2.getUpperRight();

	Point2d ll(std::min(ll1.x, ll2.x),std::min(ll1.y, ll2.y));
	Point2d ur(std::max(ur1.x, ur2.x),std::max(ur1.y, ur2.y));
	return Rect2d(ll, ur);
}

										// output operator
std::ostream& operator<<(std::ostream& out, const Rect2d& r)
{
	out << "[" << r.getLowerLeft() << " -- " << r.getUpperRight() << "]";
	return out;
}



Vector3d& Vector3d::operator+=(const Vector3d& v)
	{ x += v.x; y += v.y; z += v.z; return *this; }


Vector3d Vector3d::operator+(const Vector3d& v) const 
	{ return Vector3d(x + v.x, y + v.y, z + v.z); }