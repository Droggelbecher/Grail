
#include "actor.h"

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

