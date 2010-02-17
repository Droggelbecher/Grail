
#ifndef BOX_H
#define BOX_H

#include "animation.h"
#include "vector2d.h"
#include "surface.h"
#include <SDL/SDL.h>

namespace grail {

  class Box : public Animation {
    private:
      Surface surface;
      VirtualSize size;

    public:
      Box(VirtualSize size, SDL_Color color = black) : surface(conv<VirtualSize, PhysicalSize>(size)), size(size) {
        SDL_FillRect(surface.getSDL(), 0, SDL_MapRGB(surface.getSDL()->format, color.r, color.g, color.b));
      }

      void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
        surface.blit(PhysicalPosition(), target, conv<VirtualSize, PhysicalSize>(p));
      }

      VirtualSize getSize() const { return size; }
   };

}

#endif // BOX_H

