
#ifndef AREA_H
#define AREA_H

#include <string>
#include "vector2d.h"
#include "reference_counting.h"

class Area : public ReferenceCounted {
  public:
    static const std::string className;

    Area() { }
    virtual ~Area() { }
    virtual bool hasPoint(VirtualPosition p) const = 0;
};

#endif // AREA_H

