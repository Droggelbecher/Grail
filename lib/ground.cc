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

Ground::Ground() {
}

Ground::~Ground() {
	list<Waypoint*>::iterator iter;
	for(iter=waypoints.begin(); iter!=waypoints.end(); ++iter) {
		delete *iter;
	}
}

void Ground::addWall(VirtualPosition a, VirtualPosition b) {
	walls.push_back(Line(a, b));
}

void Ground::addWalls(const Polygon& polygon) {
	Polygon::LineIterator iter;
	
	for(iter = polygon.beginLines(); iter != polygon.endLines(); iter++) {
		walls.push_back(*iter);
	}
}

Ground::Waypoint& Ground::addWaypoint(VirtualPosition p) {
	Waypoint *wp = new Waypoint(p);
	waypoints.push_back(wp);
	return *wp;
}

bool Ground::directReachable(VirtualPosition source, VirtualPosition target) {
	Line line = Line(source, target);
	list<Line>::iterator iter;
	bool found_intersection = false;
	for(iter = walls.begin(); iter != walls.end(); iter++) {
		// It is always ok to walk along walls (Waypoint::likes will take care
		// that we do not cross them)
		if(
				(iter->getA() == source && iter->getB() == target) ||
				(iter->getB() == source && iter->getA() == target)) {
			return true;
		}
		
		if(line.intersects(*iter)) {
			found_intersection = true;
		}
	}
	
	return !found_intersection;
}

void Ground::generateMap() {
	for(list<Line>::iterator iter = walls.begin(); iter != walls.end(); iter++) {
		waypoints.push_back(new WallWaypoint(iter->getA(), iter->getB(), -1));
		waypoints.push_back(new WallWaypoint(iter->getA(), iter->getB(),  1));
		waypoints.push_back(new WallWaypoint(iter->getB(), iter->getA(), -1));
		waypoints.push_back(new WallWaypoint(iter->getB(), iter->getA(),  1));
	}
	
	// Now mesh-connect all waypoints
	for(list<Waypoint*>::iterator i = waypoints.begin(); i != waypoints.end(); ++i) {
		for(list<Waypoint*>::iterator j = waypoints.begin(); j != waypoints.end(); ++j) {
			if(directReachable((*i)->getPosition(), (*j)->getPosition())) {
				(*i)->linkBidirectional(**j);
			}
		} // for j
	} // for i
}
	

void Ground::getPath(VirtualPosition source, VirtualPosition target, Path& path) {
	generateMap();
	
	// In this class, paths directly from/to wall endpoints are generally
	// allowed (internally), since the optimal (non-trivial) path always is along walls.
	// This means however the user could "break out" by walking exactly onto a
	// wall intersection and from there he could chose any side of the wall to
	// continue. In order to avoid this, we forbid the target to be at such a
	// position.
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
		do {
			path.push_front(p->getPosition());
			p = p->cheapestParent;
		} while(p && *p != source);
	}
} // getPath

}

