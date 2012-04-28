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

void Ground::addPolygonToComponent(const Polygon<VirtualPosition, IsPosition>& polygon, Component* node) {
	if(!node && !rootComponent) {
		rootComponent = new Component(polygon);
	}
	else if(!node) {
		addPolygonToComponent(polygon, rootComponent);
	}
	else {
		VirtualPosition pos = *(polygon.beginNodes());
		for(Component::hole_iter_t iter = node->holes.begin(); iter != node->holes.end(); ++iter) {
			if((*iter)->outerBoundary.hasPoint(pos)) {
				addPolygonToComponent(polygon, *iter);
				return;
			}
		}
		node->holes.push_back(new Component(polygon));
	}
} // addPolygon

bool Ground::directReachable(Component* component, Waypoint& wp1, Waypoint& wp2) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	if(wp1 == wp2) { return true; }
	
	const polygon_t &p = component->outerBoundary;
	
	// Waypoints outside of the component are not reachable
	if(!p.hasPoint(wp1.getPosition())) { return false; }
	if(!p.hasPoint(wp2.getPosition())) { return false; }
	
	// Waypoints inside of holes are not reachable
	for(Component::hole_iter_t iter = component->holes.begin(); iter != component->holes.end(); ++iter) {
		const polygon_t &h = (*iter)->outerBoundary;
		if(h.hasPoint(wp1.getPosition()) && !h.hasBoundaryPoint(wp1.getPosition())) { return false; }
		if(h.hasPoint(wp2.getPosition()) && !h.hasBoundaryPoint(wp2.getPosition())) { return false; }
	}
	
	Line l(wp1.getPosition(), wp2.getPosition());
	
	// Even though no point is outside of the outer boundary (checked above),
	// the line between them might cross the boundary (e.g. if the boundary
	// is non-convex)
	// if l crosses the boundary that means there is no line of sight between
	// wp1 and wp2 though.
	// Ignore crossings when one of them is a boundary point though.
	
	for(polygon_t::LineIterator li = p.beginLines(); li != p.endLines(); ++li) {
		if(/*l != *li &&*/ l.intersects(*li, Line::THIS_INNER | Line::OTHER_INNER | Line::OTHER_BOUNDARY)) {
			return false;
		}
	}
	
	// Also, if $l crosses a hole, wp2 is not directly reachable from wp1.
	for(Component::hole_iter_t hi = component->holes.begin(); hi != component->holes.end(); ++hi) {
		polygon_t p = (*hi)->outerBoundary;
		for(polygon_t::LineIterator li = p.beginLines(); li != p.endLines(); ++li) {
			//if(l.intersects(*li)) {
			if(l.intersects(*li, Line::THIS_INNER | Line::OTHER_INNER | Line::OTHER_BOUNDARY)) {
				return false;
			}
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
	else if(dir == polygon_t::OUT) {
		return false;
	}
		
	for(Component::hole_iter_t hi = component->holes.begin(); hi != component->holes.end(); ++hi) {
		polygon_t::LineDirection dir = (*hi)->outerBoundary.getLineDirection(l);
		if(dir == polygon_t::IN) {
			return false;
		}
		else if(dir == polygon_t::OUT) { return true; }
	}
	
	// Both points must be inner points now that are directly reachable from
	// each other.
	return true;
}

void Ground::generateMapForComponent(Component* component, Waypoint& source, Waypoint& target) {
	if(!component) {
		if(rootComponent) {
			generateMapForComponent(rootComponent, source, target);
		}
		return;
	}
	
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	component->generateWaypoints();
	
	component->waypoints.push_back(source);
	component->waypoints.push_back(target);
	
	// connect waypoints
	
	for(Component::waypoint_iter_t wi = component->waypoints.begin(); wi != component->waypoints.end(); ++wi) {
		for(Component::waypoint_iter_t wj = wi; wj != component->waypoints.end(); ++wj) {
			if(directReachable(component, *wi, *wj) && (wi != wj)) {
				cdbg << "genMap: " << &*wi << " " << wi->getPosition()
					<< " -- " << &*wj << " " << wj->getPosition() << "\n";
				
				// TODO: connect
				//if(component->containsLine(*wi, *wj)) {
				//}
				wi->linkBidirectional(*wj);
			}
			
		} // for wj
	} // for wi
}

VirtualPosition Ground::findBoundaryPoint(VirtualPosition source, VirtualPosition target, const Ground::Component::polygon_t& poly) {
	Line l(source, target);
	VirtualPosition nearestPoint = source;
	double nearestDistance = (target - source).length();
	for(Component::polygon_t::LineIterator li = poly.beginLines(); li != poly.endLines(); ++li) {
		if(l.intersects(*li, Line::TOUCH_OR_INTERSECT)) {
			VirtualPosition p = l.intersection(*li);
			double len = (target - p).length();
			if(len < nearestDistance) {
				len = nearestDistance;
				nearestPoint = p;
			}
		}
	}
	return nearestPoint;
}

void Ground::getPath(VirtualPosition source, VirtualPosition target, Path& path) {
	
	// find innermost walkable component that contains source
	// (root component is walkable, holes of holes of walkable components are
	// walkable)
	
	Component *island = rootComponent, *c;
	do {
		c = island;
		island = 0;
		
		// it_hole = hole (not walkable)
		for(Component::hole_iter_t it_hole = c->holes.begin(); it_hole != c->holes.end(); ++it_hole) {
			// it_island = walkable area inside hole
			for(Component::hole_iter_t it_island = (*it_hole)->holes.begin(); it_island != (*it_hole)->holes.end(); ++it_island) {
				if(island->outerBoundary.hasPoint(source)) {
					island = *it_island;
					// break out of both "for" loops
					it_island = (*it_hole)->holes.end();
					it_hole = (*it_hole)->holes.end();
				}
			}
		}
	} while(island);
	
	
	// if target not in the component -> target := nearest polygon node
	// to target
	
	VirtualPosition nearTarget;
	if(c->outerBoundary.hasPoint(target)) {
		nearTarget = target;
		for(Component::hole_iter_t it_hole = c->holes.begin(); it_hole != c->holes.end(); ++it_hole) {
			if((*it_hole)->outerBoundary.hasPoint(target)) {
				nearTarget = findBoundaryPoint(source, target, (*it_hole)->outerBoundary);
				break;
			}
		}
	}
	else {
		nearTarget = findBoundaryPoint(source, target, c->outerBoundary);
	}
	
	// TODO: generate map for component that includes source and target
	
	Waypoint sourceWP(source), targetWP(nearTarget);
	
	generateMapForComponent(c, sourceWP, targetWP);
	
	// compute path
		
		cdbg << "sourceWP: " <<  sourceWP.getPosition()
			<< " neighbor count: " << sourceWP.neighbours.size() <<   "\n";
	
	getPath(sourceWP, targetWP, path);
}

void Ground::getPath(Waypoint& source, Waypoint& target, Path& path) {
	cdbg << "this is getPath\n\n";
	bool foundPath = false;
	vector<Waypoint*> border;
	set<Waypoint*> inner;
	Waypoint* cheapestNode;
	
	source.setCostSum(0);
	
	border.push_back(&source);
	push_heap(border.begin(), border.end(), Waypoint::comparePointer);
	
	while(!border.empty()) {
		cdbg << "border size: " << border.size();
		
		pop_heap(border.begin(), border.end(), Waypoint::comparePointer);
		cheapestNode = border.back();
		border.pop_back();
		
		cdbg << "cheapestNode: " <<  cheapestNode->getPosition()
			<< " neighbor count: " << cheapestNode->neighbours.size() <<   "\n";
		
		
		Waypoint::NeighbourIterator iter;
		for(iter = cheapestNode->beginNeighbours(); iter != cheapestNode->endNeighbours(); iter++) {
			cdbg << "cheapestNode neighbor: " << (*iter)->getPosition() << "\n";
		
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
			cdbg << p->getPosition() << "\n";
			path.push_front(p->getPosition());
			p = p->cheapestParent;
		} while(p && *p != source);
	}
	else {
		cdbg << "did not find path :(\n";
	}
} // getPath

//#ifdef DEBUG
//std::ostream& operator<<(std::ostream& os, const Ground::Waypoint& wp) {
	//[>
	//const Ground::WallWaypoint* wwp = dynamic_cast<const Ground::WallWaypoint*>(&wp);
	//if(wwp) {
		//os << "WallWaypoint(" << wwp->position << "," << wwp->wallEnd << "," << wwp->side << ")";
	//}
	//else {
		//os << "Waypoint(" << wp.position << ")";
	//}
	//*/
	//return os;
//}
//#endif // DEBUG


} // namespace

