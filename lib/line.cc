// vim: set noexpandtab:

#include "line.h"
#include "utils.h" // -> sgn

namespace grail {

bool Line::intersects(const Line& other) const {
	
	/*
	if(other.a == a && other.b == b) { return false; }
	if(other.a == b && other.b == a) { return false; }
	*/
	
	VirtualPosition l = b - a;
	int d1 = sgn(l.cross(other.a - a)) - sgn(l.cross(other.b - a));
	if(d1 == 0) return false;
	
	l = a - b;
	int d2 = sgn(l.cross(other.a - b)) - sgn(l.cross(other.b - b));
	if(d2 == 0) return false;
	
	l = other.b - other.a;
	int d3 = sgn(l.cross(a - other.a)) - sgn(l.cross(b - other.a));
	if(d3 == 0) return false;
	
	l = other.a - other.b;
	int d4 = sgn(l.cross(a - other.b)) - sgn(l.cross(b - other.b));
	if(d4 == 0) return false;
	
	// lines intersect at exactly one end point,
	// we count this as not intersecting
	if((abs(d1) == 1) && (abs(d2) == 1) && (abs(d3) == 1) && (abs(d4) == 1)) {
	//if((abs(d1) == 1) || (abs(d2) == 1) || (abs(d3) == 1) || (abs(d4) == 1)) {
		return false;
	}
	
	return true;
}

bool Line::hasPoint(VirtualPosition p) const {
	if(p == a || p == b) { return true; }
	//if(l.cross(p - a) != 0) { return false; }
	if(!fequal((b - a) * (p - a), (b - a).length() * (p - a).length())) { return false; }
	return (p - a).length() <= (b - a).length();
}

std::ostream& operator<<(std::ostream& os, Line l) {
	os << l.getA() << "-" << l.getB();
	return os;
}

} // namespace grail

