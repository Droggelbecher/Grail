// vim: set noexpandtab:

#ifndef POLYGON_H
#define POLYGON_H

#include <list>
using std::list;

#include "area.h"
#include "vector2d.h"
#include "line.h"

namespace grail {

class Polygon : public Area {
		list<VirtualPosition> nodes;
		
	public:
		class LineIterator {
				const Polygon *parent;
				list<VirtualPosition>::const_iterator iter;
			public:
				LineIterator() { }
				LineIterator(const Polygon* parent, list<VirtualPosition>::const_iterator iter) : parent(parent), iter(iter) { }
				void operator++() { ++iter; }
				void operator++(int) { iter++; }
				bool operator==(const LineIterator& other) const { return iter == other.iter; }
				bool operator!=(const LineIterator& other) const { return iter != other.iter; }
				Line operator*() {
					list<VirtualPosition>::const_iterator iter2;
					iter2 = iter;
					if(iter2 == parent->nodes.end()) { iter2 = (parent->nodes).begin(); }
					else { iter2++; }
					return Line(*iter, *iter2);
				}
		};
		
		Polygon();
		
		bool hasPoint(VirtualPosition p) const;
		static bool intersects(VirtualPosition q, VirtualPosition pa, VirtualPosition pb, bool right);
		
		LineIterator beginLines() const { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() const { return LineIterator(this, nodes.end()); }
		
		LineIterator beginLines() { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() { return LineIterator(this, nodes.end()); }
};

} // namespace grail

#endif // POLYGON_H

