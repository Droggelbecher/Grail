
#include "debug.h"

namespace grail {

#ifdef DEBUG
  std::ostream &cdbg = cerr;
#else
  NullStream cdbg;
#endif

} // namespace grail

