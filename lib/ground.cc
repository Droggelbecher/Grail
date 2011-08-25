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

Ground::Ground() : root(0) {
}

Ground::~Ground() {
	/*
	list<Waypoint*>::iterator iter;
	for(iter=waypoints.begin(); iter!=waypoints.end(); ++iter) {
		delete *iter;
	}
	*/
}

void Ground::addPolygon(const Polygon<VirtualPosition, IsPosition>& polygon, PolygonTreeNode* node) {
	if(!node && !root) {
		root = new PolygonTreeNode(polygon);
	}
	else if(!node) {
		addPolygon(polygon, root);
	}
	else {
		VirtualPosition pos = *(polygon.beginNodes());
		for(PolygonTreeNode::iterator_t iter = node->beginChilds(); iter != node->endChilds(); ++iter) {
			if((*iter)->getPolygon().hasPoint(pos)) {
				addPolygon(polygon, *iter);
				return;
			}
		}
		node->addChild(new PolygonTreeNode(polygon));
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

bool Ground::directReachable(VirtualPosition source, VirtualPosition target) {
	/*
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
	*/
	return false;
}

void Ground::generateMap(PolygonTreeNode* node) {
	if(!node) {
		clearMap();
		if(root) {
			generateMap(root);
		}
		return;
	}
	
	typedef Polygon<VirtualPosition, IsPosition> polygon_t;
	
	std::vector<const polygon_t*> polygons;
	polygons.push_back(&(node->getPolygon()));
	for(PolygonTreeNode::iterator_t iter = node->beginChilds(); iter != node->endChilds(); ++iter) {
		polygons.push_back(&((*iter)->getPolygon()));
	}
	
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

