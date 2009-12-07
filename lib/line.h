
#ifndef LINE_H
#define LINE_H

#include "vector2d.h"

class Line {
    VirtualPosition a, b;
  public:
    Line(VirtualPosition a, VirtualPosition b) : a(a), b(b) { }

    bool intersects(const Line& other) const;
};

#endif // LINE_H

