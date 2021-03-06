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

//void Ground::addPolygonToComponent(const Polygon<VirtualPosition, IsPosition>& polygon, Component* node) {
void Ground::addPolygonToComponent(Polygon<VirtualPosition, IsPosition>::Ptr polygon, Component* node) {
	if(!node && !rootComponent) {
		rootComponent = new Component(polygon);
	}
	else if(!node) {
		addPolygonToComponent(polygon, rootComponent);
	}
	else {
		VirtualPosition pos = *(polygon->beginNodes());
		for(Component::const_hole_iter_t iter = node->getHoles().begin(); iter != node->getHoles().end(); ++iter) {
			if((*iter)->getOuterBoundary().hasPoint(pos)) {
				addPolygonToComponent(polygon, *iter);
				return;
			}
		}
		node->addHole(new Component(polygon));
	}
} // addPolygon

bool Ground::directReachable(Component* component, Waypoint& wp1, Waypoint& wp2) {
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	if(wp1 == wp2) { return true; }
	
	const polygon_t &p = component->getOuterBoundary();
	
	// Waypoints outside of the component are not reachable
	if(!p.hasPoint(wp1.getPosition())) { return false; }
	if(!p.hasPoint(wp2.getPosition())) { return false; }
	
	// Waypoints inside of holes are not reachable
	for(Component::const_hole_iter_t iter = component->getHoles().begin(); iter != component->getHoles().end(); ++iter) {
		const polygon_t &h = (*iter)->getOuterBoundary();
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
	for(Component::const_hole_iter_t hi = component->getHoles().begin(); hi != component->getHoles().end(); ++hi) {
		const polygon_t& p = (*hi)->getOuterBoundary();
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
	
	polygon_t::LineDirection dir = component->getOuterBoundary().getLineDirection(l);
	if(dir == polygon_t::IN) { return true; }
	else if(dir == polygon_t::OUT) {
		return false;
	}
		
	for(Component::const_hole_iter_t hi = component->getHoles().begin(); hi != component->getHoles().end(); ++hi) {
		polygon_t::LineDirection dir = (*hi)->getOuterBoundary().getLineDirection(l);
		if(dir == polygon_t::IN) {
			return false;
		}
		else if(dir == polygon_t::OUT) { return true; }
	}
	
	// Both points must be inner points now that are directly reachable from
	// each other.
	return true;
}

void Ground::generateMapForComponent(Component* component, VirtualPosition src, VirtualPosition tgt, Waypoint*& source, Waypoint*& target) {
	if(!component) {
		if(rootComponent) {
			generateMapForComponent(rootComponent, src, tgt, source, target);
		}
		return;
	}
	
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	component->clearWaypoints();
	component->generateWaypoints();
	
	source = component->createWaypoint(src);
	target = component->createWaypoint(tgt);
	
	// connect waypoints
	
	for(Component::waypoint_iter_t wi = component->getWaypoints().begin(); wi != component->getWaypoints().end(); ++wi) {
		for(Component::waypoint_iter_t wj = wi; wj != component->getWaypoints().end(); ++wj) {
			if(directReachable(component, **wi, **wj) && (wi != wj)) {
				(*wi)->linkBidirectional(**wj);
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
	
	VirtualPosition p = nearestPoint; //findInnerPoint(nearestPoint, poly);
	
	cdbg << "-------- found boundary point. " << " p=" << p <<
		" orig=" << nearestPoint << " inpoly=" << poly.hasPoint(p) << "\n";
	
	return p;
	
	//double ll = 10.0 / (source - target).length(); 
	//return nearestPoint + (source - target) * ll;
	
}

VirtualPosition Ground::findInnerPoint(VirtualPosition source, const Ground::Component::polygon_t& poly) {
	int tries = 0;
	int r = 1;
	VirtualPosition p = source;
	while(!poly.hasPoint(p) && (r <= 100)) {
		for(int sx = -1; sx <= 1; sx += 2) {
			for(int y = -r; y <= r; y++) {
				tries++;
				p = source + VirtualPosition(r * sx, y);
				if(poly.hasPoint(p)) { break; }
			}
			if(poly.hasPoint(p)) { break; }
		}
		
		for(int sy = -1; sy <= 1; sy += 2) {
			for(int x = -r + 1; x < r; x++) {
				tries++;
				p = source + VirtualPosition(x, r * sy);
				if(poly.hasPoint(p)) { break; }
			}
			if(poly.hasPoint(p)) { break; }
		}
		
		r++;
	}
	
	cdbg << "rescue from point " << source << " -> " << p << " in " << tries << " tries r=" << r << ".\n";
	
	return p;
}
	

void Ground::getPath(VirtualPosition source, VirtualPosition target, Path& path) {
	
	cdbg << "Ground::getPath(" << source << ", " << target << ")\n";
	
	// find innermost walkable component that contains source
	// (root component is walkable, holes of holes of walkable components are
	// walkable)
	
	Component *island = rootComponent, *c = 0;
	do {
		c = island;
		island = 0;
		
		// it_hole = hole (not walkable)
		for(Component::const_hole_iter_t it_hole = c->getHoles().begin(); it_hole != c->getHoles().end(); ++it_hole) {
			// it_island = walkable area inside hole
			for(Component::const_hole_iter_t it_island = (*it_hole)->getHoles().begin(); it_island != (*it_hole)->getHoles().end(); ++it_island) {
				if(island->getOuterBoundary().hasPoint(source)) {
					island = *it_island;
					// break out of both "for" loops
					it_island = (*it_hole)->getHoles().end();
					it_hole = (*it_hole)->getHoles().end();
				}
			}
		}
	} while(island);
	
	if(!c) {
		cdbg << "Ground::getPath: no WA component that contains source found! --> aborting pathfinding!\n";
		return;
	}
	
	source = findInnerPoint(source, c->getOuterBoundary());
	
	// if target not in the component -> target := nearest polygon node
	// to target
	
	VirtualPosition nearTarget;
	if(c->getOuterBoundary().hasPoint(target)) {
		nearTarget = target;
		for(Component::const_hole_iter_t it_hole = c->getHoles().begin(); it_hole != c->getHoles().end(); ++it_hole) {
			if((*it_hole)->getOuterBoundary().hasPoint(target)) {
				nearTarget = findBoundaryPoint(source, target, (*it_hole)->getOuterBoundary());
				break;
			}
		}
	}
	else {
		nearTarget = findBoundaryPoint(source, target, c->getOuterBoundary());
		cdbg << "Ground::getPath: nearTarget=" << nearTarget << "\n";
		nearTarget_ = nearTarget;
	}
	
	Waypoint *sourceWP, *targetWP;
	
	generateMapForComponent(c, source, nearTarget, sourceWP, targetWP);
	
	// compute path
		
		cdbg << "Ground::getPath: sourceWP: " <<  sourceWP << " " << sourceWP->getPosition()
			<< " neighbor count: " << sourceWP->neighbours.size() <<   "\n";
	
	getPath(*sourceWP, *targetWP, path);
}

void Ground::getPath(Waypoint& source, Waypoint& target, Path& path) {
	//cdbg << "this is getPath\n\n";
	bool foundPath = false;
	vector<Waypoint*> border;
	set<Waypoint*> inner;
	Waypoint* cheapestNode;
	
	source.setCostSum(0);
	
	border.push_back(&source);
	push_heap(border.begin(), border.end(), Waypoint::comparePointer);
	
	while(!border.empty()) {
		//cdbg << "border size: " << border.size() << "\n";
		
		pop_heap(border.begin(), border.end(), Waypoint::comparePointer);
		cheapestNode = border.back();
		border.pop_back();
		
		//cdbg << "cheapestNode: " <<  cheapestNode->getPosition()
			//<< " costSum: " << cheapestNode->getCostSum() << "\n";
			//<< " neighbor count: " << cheapestNode->neighbours.size() <<   "\n";
		
		
		Waypoint::NeighbourIterator iter;
		for(iter = cheapestNode->beginNeighbours(); iter != cheapestNode->endNeighbours(); iter++) {
			//cdbg << "  neighbor: " << (*iter)->getPosition() << "\n";
		
			double cost = cheapestNode->costTo(*iter);
			double newCost = cheapestNode->getCostSum() + cost;
			
			if(inner.count(*iter) < 1) {
				// Node is totally new to us (neither in 'inner' or in 'border')
				// ==> previous cost value is infinity
				if(count(border.begin(), border.end(), *iter) < 1) {
					//cdbg << "  updating cost: infty -> " << newCost << "\n";
					
					(*iter)->setCostSum(newCost);
					(*iter)->cheapestParent = cheapestNode;
					border.push_back(*iter);
					push_heap(border.begin(), border.end(), Waypoint::comparePointer);
					make_heap(border.begin(), border.end(), Waypoint::comparePointer);
				}
				else if(newCost < (*iter)->getCostSum()) {
					//cdbg << "  updating cost: " << (*iter)->getCostSum() << " -> " << newCost << "\n";
					
					vector<Waypoint*>::iterator i;
					i = find(border.begin(), border.end(), *iter);
					border.erase(i);
					(*iter)->setCostSum(newCost);
					(*iter)->cheapestParent = cheapestNode;
					border.push_back(*iter);
					push_heap(border.begin(), border.end(), Waypoint::comparePointer);
					make_heap(border.begin(), border.end(), Waypoint::comparePointer);
				}
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

