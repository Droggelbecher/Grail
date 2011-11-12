// vim: set noexpandtab:

#include <vector>
using std::vector;
#include <set>
using std::set;
#include <limits>
using std::numeric_limits;
#include <algorithm>
using std::push_heap;
using std::pop_heap;

#include "ground.h"
#include "debug.h"

namespace grail {

Ground::Ground() : rootComponent(0) {
}

Ground::~Ground() {
	/*
	list<Waypoint*>::iterator iter;
	for(iter=waypoints.begin(); iter!=waypoints.end(); ++iter) {
		delete *iter;
	}
	*/
}

void Ground::addPolygon(const Polygon<VirtualPosition, IsPosition>& polygon, Component* node) {
	if(!node && !rootComponent) {
		rootComponent = new Component(polygon);
	}
	else if(!node) {
		addPolygon(polygon, rootComponent);
	}
	else {
		VirtualPosition pos = *(polygon.beginNodes());
		for(Component::hole_iter_t iter = node->holes.begin(); iter != node->holes.end(); ++iter) {
			if((*iter)->outerBoundary.hasPoint(pos)) {
				addPolygon(polygon, *iter);
				return;
			}
		}
		node->holes.push_back(new Component(polygon));
	}
} // addPolygon

/*
void Ground::addWalls(const Polygon<VirtualPosition, IsPosition>& polygon) {
	WaypointPolygon *inner = new WaypointPolygon(), *outer = new WaypointPolygon();
	for(Polygon<VirtualPosition, IsPosition>::ConstNodeIterator iter = polygon.beginNodes(); iter != polygon.endNodes(); ++iter) {
		// TODO: Alter directReachable so that points in inner are
		// only reachable by points inside the polygon (inside/outside test)
		// and points in outer are only reachable by points outside the
		// polygon.
		// This way this method can be used to add an enclosing outside
		// polygon as well as for obstacles (or even obstacles with holes)
		inner->push_back(new Waypoint(*iter));
		outer->push_back(new Waypoint(*iter));
	}
	innerPolygons.push_back(inner);
	outerPolygons.push_back(outer);
}
*/

/*
Ground::Waypoint& Ground::addWaypoint(VirtualPosition p) {
	Waypoint *wp = new Waypoint(p);
	waypoints.push_back(wp);
	return *wp;
}
*/

bool Ground::directReachable(Component* component, Waypoint wp1, Waypoint wp2) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	// Waypoints outside of the component are not reachable
	if(!component->outerBoundary.hasPoint(wp1.getPosition())) { cdbg << "wp1 outside\n"; return false; }
	if(!component->outerBoundary.hasPoint(wp2.getPosition())) { return false; }
	
	if(wp1 == wp2) { return true; }
	
	Line l(wp1.getPosition(), wp2.getPosition());
	
	// Even though no point is outside of the outer boundary (checked above),
	// the line between them might cross the boundary (e.g. if the boundary
	// is)
	
	// If at least one point is not on the $outerBoundary, that means $l
	// crosses $outerBoundary.
	polygon_t o = component->outerBoundary; 
	for(polygon_t::LineIterator li = o.beginLines(); li != o.endLines(); ++li) {
		if(l != *li && l.intersects(*li)) { return false; }
	}
	
	// Also, if $l crosses a hole, wp2 is not directly reachable from wp1.
	for(Component::hole_iter_t hi = component->holes.begin(); hi != component->holes.end(); ++hi) {
		polygon_t p = (*hi)->outerBoundary;
		for(polygon_t::LineIterator li = p.beginLines(); li != p.endLines(); ++li) {
			if(l != *li && l.intersects(*li)) { return false; }
		}
	}
	
	// Also, it might happen that both lie directly on the outer boundary
	// and/or holes (we assume in that case they are polygon vertices), but $l
	// is not (completely) inside the $outerBoundary polygon (because that is
	// non-convex).  -> Check whether $l extends to the inside of the polygon
	// or not.  (it's enough to check from one side (if it would behave
	// different on the other, it would cross a polygon edge)!)
	
	polygon_t::LineDirection dir = component->outerBoundary.getLineDirection(l);
	if(dir == polygon_t::IN) { return true; }
	else if(dir == polygon_t::OUT) { return false; }
		
	for(Component::hole_iter_t hi = component->holes.begin(); hi != component->holes.end(); ++hi) {
		polygon_t::LineDirection dir = (*hi)->outerBoundary.getLineDirection(l);
		if(dir == polygon_t::IN) { return false; }
		else if(dir == polygon_t::OUT) { return true; }
	}
	
	// Both points must be inner points now that are directly reachable from
	// each other.
	// TODO: ...or they may be located on a polygon edge but not on a polygon
	// node, which is a case we ignore for now)
	return true;
}

void Ground::generateMap(Component* component) {
	if(!component) {
		//clearMap();
		if(rootComponent) {
			generateMap(rootComponent);
		}
		return;
	}
	
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	// $polygons := all polygons that bound the current component
	
	std::vector<const polygon_t*> polygons;
	polygons.push_back(&(component->outerBoundary));
	for(Component::hole_iter_t iter = component->holes.begin(); iter != component->holes.end(); ++iter) {
		polygons.push_back(&((*iter)->outerBoundary));
	}
	
	// find out waypoints
	
	component->waypoints.clear();
	for(std::vector<const polygon_t*>::iterator pi = polygons.begin(); pi != polygons.end(); ++pi) {
		for(polygon_t::ConstNodeIterator ni = (*pi)->beginNodes(); ni != (*pi)->endNodes(); ++ni) {
			component->waypoints.push_back(Waypoint(*ni));
		}
	}
	
	for(Component::waypoint_iter_t wi = component->waypoints.begin(); wi != component->waypoints.end(); ++wi) {
		for(Component::waypoint_iter_t wj = wi; wj != component->waypoints.end(); ++wj) {
			if(directReachable(component, *wi, *wj)) {
				// TODO: connect
			}
			
		} // for wj
	} // for wi
	
	
	// connect waypoints
	
		// connect iff
		// - connecting edge extends to inner of outerBoundary or outer of a hole
		// - connecting edge does not cross any boundary edge
	
	
	
	//------------------
	
	
	// Let component hold all its waypoints
	/*
	component->waypoints.clear();
	for(std::vector<const polygon_t*>::iterator pi = polygons.begin(); pi != polygons.end(); ++pi) {
		for(polygon_t::ConstNodeIterator ni = (*pi)->beginNodes(); ni != (*pi)->endNodes(); ++ni) {
			component->waypoints.push_back(Waypoint(*ni));
		}
	}
	*/
	
//	for(Component::waypoint_iter_t wi = component
	
/*	
	// TODO: First, create waypoints for all polygon nodes, then iterate over
	// those!
	
	// First polygon node (niter)
	for(std::vector<const polygon_t*>::iterator piter = polygons.begin(); piter != polygons.end(); ++piter) {
		for(polygon_t::ConstNodeIterator niter = niter->beginNodes(); niter != piter->endNodes();  ++niter) {
			
			// Second polygon node (niter2)
			for(std::vector<const polygon_t*>::iterator piter2 = polygons.begin(); piter2 != polygons.end(); ++piter2) {
				for(polygon_t::ConstNodeIterator niter2 = niter2->beginNodes(); niter2 != piter2->endNodes();  ++niter2) {
					Line l(*niter, *niter2);
					
					//bool intersects = false;
					
					// Polygon lines (liter)
					for(std::vector<const polygon_t*>::iterator piter3 = polygons.begin(); piter3 != polygons.end(); ++piter3) {
						for(polygon_t::LineIterator liter = piter3->beginLines(); liter != piter3->endLines(); ++liter) {
							if(l != *liter && l.intersects(*liter)) {
					//			intersects = true;
								goto intersection_found;
							}
						}
					}
					
					wp1.linkBidirectional(wp2);
					
					intersection_found:
				} // niter2
			} // piter2
		} // niter
	} // piter
	*/
	/*
	for(list<Line>::iterator iter = walls.begin(); iter != walls.end(); iter++) {
		waypoints.push_back(new WallWaypoint(iter->getA(), iter->getB(), -1));
		waypoints.push_back(new WallWaypoint(iter->getA(), iter->getB(),  1));
		waypoints.push_back(new WallWaypoint(iter->getB(), iter->getA(), -1));
		waypoints.push_back(new WallWaypoint(iter->getB(), iter->getA(),  1));
	}
	
	// two points A,B will be connected iff:
	// - they are directReachable() (ie. the line between them doesn't cross a
	//   polygon edge
	// - i
	
	
	// Now mesh-connect all waypoints
	for(list<Waypoint*>::iterator i = waypoints.begin(); i != waypoints.end(); ++i) {
		for(list<Waypoint*>::iterator j = waypoints.begin(); j != waypoints.end(); ++j) {
			if(directReachable((*i)->getPosition(), (*j)->getPosition())) {
				(*i)->linkBidirectional(**j);
			}
		} // for j
	} // for i
	*/
}
	

void Ground::getPath(VirtualPosition source, VirtualPosition target, Path& path) {
	generateMap();
	
	// In this class, paths directly from/to wall endpoints are generally
	// allowed (internally), since the optimal (non-trivial) path always is along walls.
	// This means however the user could "break out" by walking exactly onto a
	// wall intersection and from there he could chose any side of the wall to
	// continue. In order to avoid this, we forbid the target to be at such a
	// position.
	
	/*
	list<Waypoint*>::iterator iter;
	for(iter = waypoints.begin(); iter != waypoints.end(); iter++) {
		if(target == (*iter)->getPosition()) {
			return;
		}
	}
	
	if(directReachable(source, target)) {
		path.push_back(target);
		return;
	}
	
	// Create temporary waypoints for source and target
	Waypoint *s = new Waypoint(source),
		 *t = new Waypoint(target);
	waypoints.push_back(s);
	waypoints.push_back(t);
	
	
	// Now connect them to all other reachable points
	for(iter = waypoints.begin(); iter != waypoints.end(); iter++) {
		if(*iter != t && directReachable(t->getPosition(), (*iter)->getPosition())) {
			t->linkBidirectional(**iter);
		}
		if(*iter != s && directReachable(s->getPosition(), (*iter)->getPosition())) {
			s->linkBidirectional(**iter);
		}
	}
	
	getPath(*s, *t, path);
	
	waypoints.pop_back();
	waypoints.pop_back();
	
	delete s;
	delete t;
	
	*/
}

void Ground::getPath(Waypoint& source, Waypoint& target, Path& path) {
	bool foundPath = false;
	vector<Waypoint*> border;
	set<Waypoint*> inner;
	Waypoint* cheapestNode;
	
	source.setCostSum(0);
	
	border.push_back(&source);
	push_heap(border.begin(), border.end(), Waypoint::comparePointer);
	
	while(!border.empty()) {
		pop_heap(border.begin(), border.end(), Waypoint::comparePointer);
		cheapestNode = border.back();
		border.pop_back();
		
		Waypoint::NeighbourIterator iter;
		for(iter = cheapestNode->beginNeighbours(); iter != cheapestNode->endNeighbours(); iter++) {
			double cost = cheapestNode->costTo(*iter);
			double newCost = cheapestNode->getCostSum() + cost;
			
			// Node is totally new to us (neither in 'inner' or in 'border')
			if(inner.count(*iter) < 1 && count(border.begin(), border.end(), *iter) < 1) {
				(*iter)->setCostSum(newCost);
				(*iter)->cheapestParent = cheapestNode;
				border.push_back(*iter);
				push_heap(border.begin(), border.end(), Waypoint::comparePointer);
			}
			else if(count(border.begin(), border.end(), *iter) > 0) {
				vector<Waypoint*>::iterator i;
				i = find(border.begin(), border.end(), *iter);
				border.erase(i);
				(*iter)->setCostSum(newCost);
				(*iter)->cheapestParent = cheapestNode;
				border.push_back(*iter);
				push_heap(border.begin(), border.end(), Waypoint::comparePointer);
				make_heap(border.begin(), border.end(), Waypoint::comparePointer);
			}
		} // for
		
		inner.insert(cheapestNode);
		if(cheapestNode == &target) {
			foundPath = true;
		}
	} // while
	
	if(foundPath) {
		Waypoint* p = &target;
		cdbg << "found path:\n";
		do {
			cdbg << *p << "\n";
			path.push_front(p->getPosition());
			p = p->cheapestParent;
		} while(p && *p != source);
	}
} // getPath

#ifdef DEBUG
std::ostream& operator<<(std::ostream& os, const Ground::Waypoint& wp) {
	/*
	const Ground::WallWaypoint* wwp = dynamic_cast<const Ground::WallWaypoint*>(&wp);
	if(wwp) {
		os << "WallWaypoint(" << wwp->position << "," << wwp->wallEnd << "," << wwp->side << ")";
	}
	else {
		os << "Waypoint(" << wp.position << ")";
	}
	*/
	return os;
}
#endif // DEBUG


} // namespace

