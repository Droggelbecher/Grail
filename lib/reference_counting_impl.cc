
#include "animation.h"
#include "area.h"
#include "actor.h"

#include "reference_counting.cc"

namespace grail {

template class Reference<ReferenceCountingTest>;
template class Reference<Actor>;
template class Reference<Area>;
template class Reference<Animation>;

} // namespace grail

