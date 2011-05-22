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
		list<Node> nodes;
		
	public:
		class LineIterator {
				const Polygon *parent;
				typename list<Node>::const_iterator iter;
			public:
				LineIterator() { }
				LineIterator(const Polygon* parent, typename list<Node>::const_iterator iter) :
					parent(parent), iter(iter) { }
				void operator++() { ++iter; }
				void operator++(int) { iter++; }
				bool operator==(const LineIterator& other) const { return iter == other.iter; }
				bool operator!=(const LineIterator& other) const { return iter != other.iter; }
				Line operator*() {
					typename list<Node>::const_iterator iter2;
					iter2 = iter;
					if(iter2 == parent->nodes.end()) { iter2 = (parent->nodes).begin(); }
					else { iter2++; }
					return Line(GetPosition::getPosition(*iter), GetPosition::getPosition(*iter2));
				}
		};
		typedef typename list<Node>::iterator NodeIterator;
		typedef typename list<Node>::const_iterator ConstNodeIterator;
		
		Polygon();
		
		bool hasPoint(VirtualPosition p) const;
		
		LineIterator beginLines() const { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() const { return LineIterator(this, nodes.end()); }
		LineIterator beginLines() { return LineIterator(this, nodes.begin()); }
		LineIterator endLines() { return LineIterator(this, nodes.end()); }
		
		ConstNodeIterator beginNodes() const { return nodes.begin(); }
		ConstNodeIterator endNodes() const { return nodes.end(); }
		NodeIterator beginNodes() { return nodes.begin(); }
		NodeIterator endNodes() { return nodes.end(); }
		
		void push_back(const Node& p) { nodes.push_back(p); }
		
	private:
		static bool intersects(VirtualPosition q, VirtualPosition pa, VirtualPosition pb, bool right);
};

struct IsPosition {
	static VirtualPosition getPosition(const VirtualPosition& p) { return p; }
};

} // namespace grail

#endif // POLYGON_H

