
#include "vector2d.h"
#include "ground.h"

#include "polygon.cc"


namespace grail {
	
	template class Polygon<VirtualPosition, IsPosition>;
	template class Polygon<Ground::Waypoint*, Ground::Waypoint::GetPosition>;
	
} // namespace grail

