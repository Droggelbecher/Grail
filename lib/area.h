
#ifndef AREA_H
#define AREA_H

#include "vector2d.h"

class Area {
  public:
    Area() { }
    virtual ~Area() { }
    virtual bool hasPoint(VirtualPosition p) const = 0;
};

#endif // AREA_H

