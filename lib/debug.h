
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>

#ifdef DEBUG
#define cdbg cerr
#else
struct DebugStream {
  template <typename T>
  DebugStream& operator<<(T obj) {
  }
} cdbg;
#endif

#endif // DEBUG_H

