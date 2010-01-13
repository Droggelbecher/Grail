
#ifndef LINE_H
#define LINE_H

#include "vector2d.h"

namespace grail {

class Line {
    VirtualPosition a, b;
  public:
    Line(VirtualPosition a, VirtualPosition b) : a(a), b(b) { }

    bool intersects(const Line& other) const;
};

} // namespace grail

#endif // LINE_H

