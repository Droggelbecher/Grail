// vim: set noexpandtab:

#ifndef AREA_H
#define AREA_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "vector2d.h"

namespace grail {

class Area {
	public:
		typedef boost::shared_ptr<Area> Ptr;
		
		static const std::string className;
		
		Area() { }
		virtual ~Area() { }
		virtual bool hasPoint(VirtualPosition p) const = 0;
};

}

#endif // AREA_H

