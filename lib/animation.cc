
#include "animation.h"

void Animation::setAlignment(double x, double y) {
  alignmentX = x;
  alignmentY = y;
}

VirtualPosition Animation::getUpperLeftCorner(VirtualPosition p) const {
  return VirtualPosition(
      p.getX() - alignmentX * getSize().getX(),
      p.getY() - alignmentY * getSize().getY()
      );
}

