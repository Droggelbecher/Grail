
#include "actor.h"

#include <algorithm>
using std::copy;
#include <cassert>

void Actor::setAlignment(double x, double y) {
  alignmentX = x;
  alignmentY = y;
}

VirtualPosition Actor::getUpperLeftCorner() const {
  if(animation) {
    return VirtualPosition(
        position.getX() - alignmentX * animation->getSize().getX(),
        position.getY() - alignmentY * animation->getSize().getY()
        );
  }
  else {
    return position;
  }
}

void Actor::walk(const Path& path) {
  walkPath.clear();
  copy(path.begin(), path.end(), walkPath.begin());
  assert(walkPath.size() == path.size());
}

void Actor::walkStraight(VirtualPosition p) {
  walkPath.clear();
  walkPath.push_back(p);
}

