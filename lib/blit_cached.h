
#ifndef BLIT_CACHED_H
#define BLIT_CACHED_H

#include <SDL.h>
#include "vector2d.h"
#include "surface.h"

namespace grail {

  class BlitCached {
    protected:
      bool changed;
      int n;
      Surface::Ptr *surface;

    public:

      BlitCached(int n = 1);
      virtual ~BlitCached();

      void setChanged() { changed = true; }
      bool hasChanged() { return changed; }

      virtual Surface* render(int n) const = 0;

      void eachFrame(uint32_t ticks);
      void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
  };
}

#endif // BLIT_CACHED_H

