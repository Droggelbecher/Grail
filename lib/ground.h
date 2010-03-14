// vim: set noexpandtab:

#ifndef GROUND_H
#define GROUND_H

#include <string>
#include <list>
using std::list;

#include "vector2d.h"
#include "polygon.h"
#include "actor.h" // ->Path
#include "line.h"

namespace grail {

/**
 * The "ground" of a scene. This defines, from where you can go where and how.
 */
class Ground {
	public:
		struct Waypoint {
			public:
				typedef list<Waypoint*>::iterator NeighbourIterator;
				
			private:
				VirtualPosition position;
				list<Waypoint*> neighbours;
				double costSum;
				
				Waypoint(const Waypoint&) { }
				Waypoint& operator=(const Waypoint&) { return *this; }
				
				Waypoint(VirtualPosition position) : position(position), costSum(0) { }
				
			public:
				Waypoint* cheapestParent;
				
				/**
				 */
				VirtualPosition getPosition() const { return position; }
				bool operator==(const Waypoint& other) const { return position == other.position; }
				bool operator!=(const Waypoint& other) const { return position != other.position; }
				
				/**
				 * create a link this->other
				 */
				void link(Waypoint& other) { neighbours.push_back(&other); }
				
				/**
				 * create a bi directional link this<->other
				 */
				void linkBidirectional(Waypoint& other) {
					link(other);
					other.link(*this);
				}
				
				/**
				 */
				void setCostSum(double s) { costSum = s; }
				
				/**
				 */
				double getCostSum() { return costSum; }
				
				/**
				 */
				double costTo(Waypoint* other) {
					return (position - other->position).length();
				}
				
				static int comparePointer(Waypoint* a, Waypoint* b) {
					return a->costSum - b->costSum;
				}
				
				NeighbourIterator beginNeighbours() { return neighbours.begin(); }
				NeighbourIterator endNeighbours() { return neighbours.end(); }
				
				friend class Ground;
		};
		
	private:
		list<Line> walls;
		list<Waypoint*> waypoints;
		
	public:
		Ground();
		~Ground();
		
		/**
		 * Add a wall to this ground reaching from VirtualPosition a to VirtualPosition b.
		 * If there is already a connections between to nodes crossing the
		 * new wall it will still be possible to walk along this connection walking
		 * through the wall.
		 * (Rule of thumb: Connections have always priority over walls)
		 */
		void addWall(VirtualPosition a, VirtualPosition b);
		
		/**
		 * Add walls from the edges of the given polygon.
		 */
		void addWalls(const Polygon& polygon);
		
		const list<Line>& getWalls() const { return walls; }
		
		/**
		 * Add a new waypoint. Note that this will be pretty useless if you
		 * dont connect it to at least one other waypoint.
		 * Return the created Waypoint node.
		 * You might want to call that nodes .link(other_node) for a one-way link
		 * or use Ground.connect(node_a, node_b) to create a bidirectional link.
		 */
		Waypoint& addWaypoint(VirtualPosition p);
		
		/// ditto.
		///Waypoint& addWaypoint(Waypoint& wp);
		
		/**
		 * Remove a waypoint.
		 */
		//void removeWaypoint(VirtualPosition p);
		
		/**
		 * ditto.
		 */
		//void removeWaypoint(Waypoint& n);
		
		/**
		 * Returns true if point target can be reached in a direct line from source,
		 * ie there are no walls between them.
		 * (This totally ignores any possible "help" from nodes)
		 */
		bool directReachable(VirtualPosition source, VirtualPosition target);
		
		/**
		 * Returns a points of nodes that discribe a path from source to target.
		 */
		void getPath(VirtualPosition source, VirtualPosition target, Path& path);
		void getPath(Waypoint& source, Waypoint& target, Path& path);
		
};

} // namespace grail

#endif // GROUND_H

