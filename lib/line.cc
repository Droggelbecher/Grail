// vim: set noexpandtab:

#include "line.h"
#include "utils.h" // -> sgn

namespace grail {

bool Line::intersects(const Line& other, int flags) const {
	
	// if one line is not included, it can't intersect the other ;)
	
	if(!(flags & (THIS_INNER | THIS_BOUNDARY))) { return false; }
	if(!(flags & (OTHER_INNER | OTHER_BOUNDARY))) { return false; }
	
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
	
	//printf("f=%d d1=%d d2=%d d3=%d d4=%d\n", flags, d1, d2, d3, d4);
	
	// abs(d1) == 1  --> intersection is endpoint of $other
	// abs(d2) == 1  --> intersection is endpoint of $other
	// abs(d3) == 1  --> intersection is endpoint of this
	// abs(d4) == 1  --> intersection is endpoint of this
	// if == 2, inner
	// abs(d?) == 0 ruled out before

	bool d12ok = ((flags & OTHER_BOUNDARY) && ((abs(d1) == 1) && (abs(d2) == 1))) ||
			((flags & OTHER_INNER) && ((abs(d1) == 2) && (abs(d2) == 2)));
	bool d34ok = ((flags & THIS_BOUNDARY) && ((abs(d3) == 1) && (abs(d4) == 1))) ||
			((flags & THIS_INNER) && ((abs(d3) == 2) && (abs(d4) == 2)));
	
	return d12ok && d34ok;
}

VirtualPosition Line::intersection(const Line& other) const {
	// source: http://local.wasp.uwa.edu.au/~pbourke/geometry/lineline2d/
	
	double x1 = a.getX(), x2 = b.getX(), x3 = other.a.getX(), x4 = other.b.getX();
	double y1 = a.getY(), y2 = b.getY(), y3 = other.a.getY(), y4 = other.b.getY();
	
	double numerator = (x4 - x3)*(y1 - y3) - (y4 - y3)*(x1 - x3);
	double denominator = (y4 - y3)*(x2 - x1) - (x4 - x3)*(y2 - y1);
	
	assert((denominator != 0.0) && "lines parallel!");
	double ua = numerator / denominator;
	
	return VirtualPosition(x1 + ua*(x2 - x1), y1 + ua*(y2 - y1));
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

