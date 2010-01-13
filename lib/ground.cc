
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

const std::string Ground::className = "Ground";

Ground::Ground() {
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
  addWaypoint(*wp);
  return *wp;
}

Ground::Waypoint& Ground::addWaypoint(Waypoint& wp) {
  waypoints.push_back(&wp);
  return wp;
}

bool Ground::directReachable(VirtualPosition source, VirtualPosition target) {
  Line line = Line(source, target);
  list<Line>::iterator iter;
  for(iter = walls.begin(); iter != walls.end(); iter++) {
    if(line.intersects(*iter)) {
      return false;
    }
  }
  return true;
}

void Ground::getPath(VirtualPosition source, VirtualPosition target, Path& path) {
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
  list<Waypoint*>::iterator iter;
  for(iter = waypoints.begin(); iter != waypoints.end(); iter++) {
    if(*iter != t && directReachable(t->getPosition(), (*iter)->getPosition())) {
      t->link(**iter);
      (*iter)->link(*t);
    }
    if(*iter != s && directReachable(s->getPosition(), (*iter)->getPosition())) {
      s->link(**iter);
      (*iter)->link(*s);
    }
  }

  getPath(*s, *t, path);

  waypoints.pop_back();
  waypoints.pop_back();
}

void Ground::getPath(Waypoint& source, Waypoint& target, Path& path) {
  bool foundPath = false;
  vector<Waypoint*> border;
  set<Waypoint*> inner;
  //double cheapest = numeric_limits<double>::infinity();
  Waypoint* cheapestNode;

  source.setCostSum(0);

  border.push_back(&source);
  push_heap(border.begin(), border.end(), Waypoint::comparePointer);

  while(!border.empty()) {
    cheapestNode = border[0];
    pop_heap(border.begin(), border.end(), Waypoint::comparePointer);

    Waypoint::NeighbourIterator iter;
    for(iter = cheapestNode->beginNeighbours(); iter != cheapestNode->endNeighbours(); iter++) {
      double cost = cheapestNode->costTo(*iter);
      double newCost = cheapestNode->getCostSum() + cost;

      // Node is totally new to us
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
        //push_heap(border.begin(), border.end(), Waypoint::comparePointer);
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

