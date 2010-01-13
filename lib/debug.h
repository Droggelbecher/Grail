
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

namespace grail {

#ifdef DEBUG
extern std::ostream &cdbg;
#else
struct NullStream {
  template <typename T>
  NullStream& operator<<(T obj) {
    return *this;
  }
};
extern NullStream cdbg;
#endif

}

#endif // DEBUG_H

