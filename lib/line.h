// vim: set noexpandtab:

#ifndef LINE_H
#define LINE_H

#include "vector2d.h"

namespace grail {

class Line {
		VirtualPosition a, b;
	public:
		Line(VirtualPosition a, VirtualPosition b) : a(a), b(b) { }
		
		bool intersects(const Line& other) const;
		
		VirtualPosition getA() const { return a; }
		VirtualPosition getB() const { return b; }
		
		bool operator==(const Line& other) {
			return (a == other.a && b == other.b) ||
				(a == other.b && b == other.a);
		}
		bool operator!=(const Line& other) { return !(*this == other); }
};

} // namespace grail

#endif // LINE_H

