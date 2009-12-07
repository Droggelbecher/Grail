
#include <list>
#include "polygon.h"
#include "vector2d.h"

Polygon::Polygon() {
}

bool Polygon::hasPoint(VirtualPosition p) const {
  // If there is an uneven number of intersections
  // in both directions, the Offset is inside the
  // polygon
  bool unevenIntersectionsRight = false;
  bool unevenIntersectionsLeft = false;

  // Consider all lines from one point to the next one
  // plus one line from the last one to the first
  list<VirtualPosition>::const_iterator iter, iter2;
  for(iter = nodes.begin(); iter != nodes.end(); iter++) {
    iter2 = iter;
    iter2++;
    if(iter2 == nodes.end()) { iter2 = nodes.begin(); }
    unevenIntersectionsRight ^= intersects(p, *iter, *iter2, true);
    unevenIntersectionsLeft ^= intersects(p, *iter, *iter2, false);
  }
  return unevenIntersectionsRight && unevenIntersectionsLeft;
}

bool Polygon::intersects(VirtualPosition q, VirtualPosition pa, VirtualPosition pb, bool right) {
  // Think a horizontal line drawn from q to the right and look
  // for an intersection.
  VirtualPosition::Y dy = pb.getY() - pa.getY();

  // If dy is 0, we test against a horizontal line. In this border-case we
  // define the lines as non-intersecting.
  if(dy == 0)
    return false;

  // We assumed endless straight lines up to now. Now look if the
  // intersection point is really between pa and pb.
  // t == 0 would mean: The intersection point is pa, t == 1 would mean the
  // intersection point is pb.
  double t = (double)(q.getY() - pa.getY()) / (double)dy;
  if(t < 0.0 || t > 1.0)
    return false;

  if(t == 0.0 || t == 1.0) {
    // DARN! We hit a node. To make sure we didnt just strive the polygon,
    // consider it as intersection if the other point has a lower y-value.
    // (See http://alienryderflex.com/polygon/)

    if(!((t == 0.0 && pa.getY() > pb.getY()) || (t == 1.0 && pb.getY() > pa.getY()))) {
      return false;
    }
  }

  // w is the distance from q to the intersection point.
  // If it is not positive, we would have to draw our line to the left, but
  // we only consider one drawn to the right.
  VirtualPosition::X w = pa.getX() - q.getX() + (pb.getX() - pa.getX()) * t;
  if(right)
    return w > 0;
  else
    return w < 0;
} // intersects()

