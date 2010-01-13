
#include "debug.h"

namespace grail {

#ifdef DEBUG
  std::ostream &cdbg = std::cerr;
#else
  NullStream cdbg;
#endif

} // namespace grail

