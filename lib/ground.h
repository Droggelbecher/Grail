// vim: set noexpandtab:

#ifndef GROUND_H
#define GROUND_H

#include "visualize.h"

#include <string>
#include <list>
#include <vector>
using std::list;
#include <iostream>

#include "vector2d.h"
#include "polygon.h"
#include "actor.h" // ->Path
#include "line.h"
#include "utils.h" // ->sgn

namespace grail {

/**
 * The "ground" of a scene. This defines, from where you can go where and how.
 * 
 * The current pathfinding implementation is improveable performance-wise
 * (e.g. we don't use A*, only dijkstra, we don't pre-calculate a
 * shortest-path map, etc...) However for the expected number of waypoints
 * this will handle, this should be more than suffcient.
 */
class Ground {
	public:
		class Waypoint {
			public:
				typedef list<Waypoint*>::iterator NeighbourIterator;
				
				VirtualPosition position;
				list<Waypoint*> neighbours;
				double costSum;
				
				Waypoint(const Waypoint& other)
					: position(other.position), neighbours(other.neighbours), costSum(other.costSum) { }
				Waypoint& operator=(const Waypoint& other) {
					position = other.position;
					neighbours = other.neighbours;
					costSum = other.costSum;
					return *this;
				}
				
			public:
				Waypoint(VirtualPosition position) : position(position), costSum(0), cheapestParent(0) { }
				virtual ~Waypoint() { reset(); }
				
				void link(Waypoint& other) { neighbours.push_back(&other); }
				void unlink(Waypoint& other) {/* neighbours.remove(&other);*/ }
				
				Waypoint* cheapestParent;
				
				struct GetPosition {
					static VirtualPosition getPosition(const Waypoint *wp) { return wp->getPosition(); }
				};
				
				void reset() {
					cheapestParent = 0;
					NeighbourIterator iter;
					for(iter = beginNeighbours(); iter != endNeighbours(); ++iter) {
						(*iter)->unlink(*this);
					}
				}
				
				VirtualPosition getPosition() const { return position; }
				bool operator==(const Waypoint& other) const { return position == other.position; }
				bool operator!=(const Waypoint& other) const { return position != other.position; }
				
				void linkBidirectional(Waypoint& other) {
					link(other);
					other.link(*this);
				}
				
				void setCostSum(double s) { costSum = s; }
				double getCostSum() { return costSum; }
				double costTo(Waypoint* other) { return (position - other->position).length(); }
				static int comparePointer(Waypoint* a, Waypoint* b) {
					return a->costSum > b->costSum;
				}
				NeighbourIterator beginNeighbours() { return neighbours.begin(); }
				NeighbourIterator endNeighbours() { return neighbours.end(); }
				
				bool hasConnection(Waypoint* other) {
					for(NeighbourIterator iter = beginNeighbours(); iter != endNeighbours(); ++iter) {
						if(*iter == other) { return true; }
					}
					return false;
				}
				
			friend class Ground;
		};
		
		/**
		 * A connected piece of ground area, bounded by one polygon for the
		 * outer bondary (@ref outerBoundary) and an arbitrary number of
		 * polygons for the inner boundary (@ref holes).
		 */
		struct Component {
			public:
				typedef Polygon<VirtualPosition, IsPosition> polygon_t;
				typedef std::vector<Component*>::iterator hole_iter_t;
				typedef std::vector<Component*>::const_iterator const_hole_iter_t;
				typedef std::vector<Waypoint*>::iterator waypoint_iter_t;
				typedef std::vector<Waypoint*>::const_iterator const_waypoint_iter_t;
			
			private:
				//const polygon_t &outerBoundary;
				polygon_t::ConstPtr outerBoundary;
				std::vector<Component*> holes;
				std::vector<Waypoint*> waypoints;
				
			public:
				//Component(const polygon_t& ob) : outerBoundary(&ob) { }
				Component(polygon_t::ConstPtr ob) : outerBoundary(ob) { }
				
				const polygon_t& getOuterBoundary() { return *outerBoundary; }
				const std::vector<Component*>& getHoles() { return holes; }
				std::vector<Waypoint*>& getWaypoints() { return waypoints; }
				
				void addHole(Component* hole) {
					holes.push_back(hole);
				}
				
				void addWaypoint(const Waypoint& wp) {
					waypoints.push_back(new Waypoint(wp));
				}
				
				Waypoint* createWaypoint(VirtualPosition p) {
					Waypoint *r = new Waypoint(p);
					waypoints.push_back(r);
					return r;
				}
				
				void clearWaypoints() {
					for(waypoint_iter_t i = waypoints.begin(); i != waypoints.end(); ++i) {
						delete *i;
					}
					waypoints.clear();
				}
				
				void generateWaypoints() {
					std::vector<const polygon_t*> polygons;
					polygons.push_back(&*(outerBoundary));
					for(Component::hole_iter_t iter = holes.begin(); iter != holes.end(); ++iter) {
						polygons.push_back(&*((*iter)->outerBoundary));
					}
	
					//waypoints.clear();
					for(std::vector<const polygon_t*>::iterator pi = polygons.begin(); pi != polygons.end(); ++pi) {
						for(polygon_t::ConstNodeIterator ni = (*pi)->beginNodes(); ni != (*pi)->endNodes(); ++ni) {
							waypoints.push_back(new Waypoint(*ni));
						}
					}
				}
				
				#if VISUALIZE_GROUND
				void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
					#if VISUALIZE_COMPONENTS
					outerBoundary->renderAt(target, ticks, p);
					for(const_hole_iter_t iter = holes.begin(); iter != holes.end(); ++iter) {
						(*iter)->renderAt(target, ticks, p);
					}
					#endif
					
					#if VISUALIZE_MAP
					for(Component::const_waypoint_iter_t wi = waypoints.begin(); wi != waypoints.end(); ++wi) {
						for(Component::const_waypoint_iter_t wj = wi; wj != waypoints.end(); ++wj) {
							
							if((*wi)->hasConnection(*wj)) {
								PhysicalPosition a = conv<VirtualPosition, PhysicalPosition>((*wi)->getPosition() + p);
								PhysicalPosition b = conv<VirtualPosition, PhysicalPosition>((*wj)->getPosition() + p);
								aalineColor(target, a.getX(), a.getY(), b.getX(), b.getY(), 0xffffff40);
							}
						}
					}
					#endif
					
				}
				#endif
		};
		
		Ground();
		~Ground();
		
		/**
		 * Add walls from the edges of the given polygon.
		 * 
		 * @pre {
		 * 	@ref polygon does not intersect any polygon added before.
		 * 	Else the behaviour is undefined
		 * }
		 * 
		 * @param polygon The polygon to add.
		 * @param node Only used internally, leave at 0.
		 */
		void addPolygon(Polygon<VirtualPosition, IsPosition>::Ptr polygon) { addPolygonToComponent(polygon, 0); }
		void addPolygonToComponent(Polygon<VirtualPosition, IsPosition>::Ptr polygon, Component* node);
		
		/**
		 * Call this after having added all needed walls/polygons.
		 * It will generate waypoints for all corners and connect them
		 * appropriately to allow pathfinding.
		 * 
		 * @param component the component to construct waypoints for
		 * @param src source position for navigation
		 * @param tgt target position for navigation
		 * @param source used to return address of constructed source waypoint
		 * @param target used to return address of constructed target waypoint
		 * 
		 */
		void generateMapForComponent(Component* component, VirtualPosition src, VirtualPosition tgt, Waypoint*& source, Waypoint*& target);
		
		/**
		 */
		bool directReachable(Component*, Waypoint&, Waypoint&);
		
		/**
		 * Returns a points of nodes that discribe a path from source to target.
		 */
		void getPath(VirtualPosition source, VirtualPosition target, Path& path);
		
		/// ditto.
		void getPath(Waypoint& source, Waypoint& target, Path& path);
		
		#if VISUALIZE_GROUND
		void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
			if(rootComponent) {
				rootComponent->renderAt(target, ticks, p);
			}
					// DEBUG
					PhysicalPosition x = conv<VirtualPosition, PhysicalPosition>(nearTarget_ + p);
					aalineColor(target, x.getX() - 5, x.getY() - 5, x.getX() + 5, x.getY() + 5, 0xffff00ff);
					aalineColor(target, x.getX() - 5, x.getY() + 5, x.getX() + 5, x.getY() - 5, 0xffff00ff);
					
		}
		#endif
		
	#if DEBUG
	public:
	#else
	private:
	#endif
		
		VirtualPosition findBoundaryPoint(VirtualPosition source, VirtualPosition target, const Component::polygon_t& poly);
		VirtualPosition findInnerPoint(VirtualPosition p, const Ground::Component::polygon_t& poly);
		
	public:
		Component *rootComponent;
		VirtualPosition nearTarget_;
};

} // namespace grail

#endif // GROUND_H

