// vim: set noexpandtab:

#ifndef POLYGON_H
#define POLYGON_H

#include <list>
using std::list;

#include "area.h"
#include "vector2d.h"
#include "line.h"

namespace grail {

/**
 */
template<typename Node, typename GetPosition>
class Polygon : public Area {
	public:
		enum Orientation { UNKNOWN, CW, CCW };
		enum LineDirection { NOT_ATTACHED, IN, OUT };
		
		class LineIterator {
				const Polygon *parent;
				typename list<Node>::const_iterator iter;
			public:
				LineIterator() : parent(0) { }
				LineIterator(const Polygon* parent, typename list<Node>::const_iterator iter) :
					parent(parent), iter(iter) { }
				void operator++() { ++iter; }
				void operator++(int) { iter++; }
				bool operator==(const LineIterator& other) const { return iter == other.iter; }
				bool operator!=(const LineIterator& other) const { return iter != other.iter; }
				Line operator*() {
					typename list<Node>::const_iterator iter2;
					iter2 = iter;
					++iter2;
					if(iter2 == parent->nodes.end()) { iter2 = (parent->nodes).begin(); }
					return Line(GetPosition::getPosition(*iter), GetPosition::getPosition(*iter2));
				}
		};
		typedef typename list<Node>::iterator NodeIterator;
		typedef typename list<Node>::const_iterator ConstNodeIterator;
		
		Polygon();
		
		/**
		 * \return true iff $p is part of the interior of this polygon.
		 */
		bool hasPoint(VirtualPosition p) const;
		
		/**
		 * \return true iff $p is part of the boundary of this polygon.
		 */
		bool hasBoundaryPoint(VirtualPosition p) const;
		
		Orientation getOrientation() const {
			if(orientation == UNKNOWN) { updateOrientation(); }
			return orientation;
		}
		
		LineDirection getLineDirection(Line l) const;
		
		LineIterator beginLines() const { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() const { return LineIterator(this, nodes.end()); }
		LineIterator beginLines() { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() { return LineIterator(this, nodes.end()); }
		
		ConstNodeIterator beginNodes() const { return nodes.begin(); }
		ConstNodeIterator endNodes() const { return nodes.end(); }
		NodeIterator beginNodes() { return nodes.begin(); }
		NodeIterator endNodes() { return nodes.end(); }
		
		void push_back(const Node& p) { nodes.push_back(p); orientation = UNKNOWN; }
		void clear() { nodes.clear(); orientation = UNKNOWN; }
		
	private:
		static bool intersects(VirtualPosition q, VirtualPosition pa, VirtualPosition pb, bool right);
		
		void updateOrientation() const;
		
		list<Node> nodes;
		mutable Orientation orientation;
		
};

struct IsPosition {
	static VirtualPosition getPosition(const VirtualPosition& p) { return p; }
};

} // namespace grail

#endif // POLYGON_H

