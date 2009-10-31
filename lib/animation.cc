
#include "animation.h"

void Animation::setAlignment(double x, double y) {
  alignmentX = x;
  alignmentY = y;
}

InternalPosition Animation::getUpperLeftCorner(InternalPosition p) {
  return InternalPosition(
      p.x - alignmentX * getWidth(),
      p.y - alignmentY * getHeight()
      );
}

