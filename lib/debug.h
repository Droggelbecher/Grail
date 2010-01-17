
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <string>

namespace grail {

#ifdef DEBUG
extern std::ostream &cdbg;
#else
struct NullStream {
};

template <typename T>
NullStream& operator<<(NullStream& s, T obj) {
  return s;
}

template NullStream& operator<<(NullStream&, const char*);
template NullStream& operator<<(NullStream&, std::string);

/*
NullStream& operator<<(NullStream& s, const char* obj) {
  return s;
}
*/

extern NullStream cdbg;
#endif

}

#endif // DEBUG_H

