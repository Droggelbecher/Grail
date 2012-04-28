// vim: set noexpandtab:

#ifndef LINE_H
#define LINE_H

#include <ostream>
#include "vector2d.h"

namespace grail {
	
class Line {
		VirtualPosition a, b;
	public:
		
		enum {
			THIS_INNER = 0x01,
			THIS_BOUNDARY = 0x02,
			OTHER_INNER = 0x04,
			OTHER_BOUNDARY = 0x08,
			
			TOUCH_OR_INTERSECT = THIS_INNER | THIS_BOUNDARY | OTHER_INNER | OTHER_BOUNDARY,
			REAL_INTERSECT = THIS_INNER | OTHER_INNER
		} Flags;
		
		Line(VirtualPosition a, VirtualPosition b) : a(a), b(b) { }
		
		bool intersects(const Line& other, int flags = REAL_INTERSECT) const;
		
		VirtualPosition getA() const { return a; }
		VirtualPosition getB() const { return b; }
		
		bool operator==(const Line& other) const {
			return (a == other.a && b == other.b) ||
				(a == other.b && b == other.a);
		}
		bool operator!=(const Line& other) const { return !(*this == other); }
		
		bool hasPoint(VirtualPosition p) const ;
};

std::ostream& operator<<(std::ostream&, Line);

} // namespace grail

#endif // LINE_H

