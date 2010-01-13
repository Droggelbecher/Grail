
#ifndef RECT_H
#define RECT_H

#include "area.h"

namespace grail {

class Rect : public Area {
    VirtualPosition topLeft;
    VirtualSize size;

  public:
    Rect(VirtualPosition topLeft, VirtualSize size) : topLeft(topLeft), size(size) { }
    Rect(VirtualSize size) : topLeft(VirtualPosition(0, 0)), size(size) { }

    bool hasPoint(VirtualPosition p) const {
      return p.getX() >= topLeft.getX() &&
        p.getX() < (topLeft + size).getX() &&
        p.getY() >= topLeft.getY() &&
        p.getY() < (topLeft + size).getY();
    }
};

} // namespace grail

#endif // RECT_H

