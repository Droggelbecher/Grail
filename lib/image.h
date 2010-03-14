// vim: set ft=noexpandtab:

#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <iostream>
#include <cassert>
using std::cerr;
using std::endl;

#include <SDL.h>

#include "surface.h"
#include "vector2d.h"
#include "animation.h"

namespace grail {

class Image : public Animation {
	protected:
		Surface::Ptr surface;
		
	public:
		Image(std::string path) : surface(new Surface(path)) {
		}
		
		virtual ~Image() {
		}
		
		VirtualSize getSize() const {
			return conv<PhysicalSize, VirtualSize>(surface->getSize());
		}
		
		void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
			SDL_Rect t = conv<PhysicalPosition, SDL_Rect>(
					conv<VirtualPosition, PhysicalPosition>(p)
					);
			surface->blit(0, target, &t);
		}
		
		bool hasPoint(VirtualPosition p) const {
			return surface->getAlpha(conv<VirtualPosition, PhysicalPosition>(p)) > 127;
		}
};

} // namespace grail

#endif // IMAGE_H

