
#include "blit_cached.h"

namespace grail {

  BlitCached::BlitCached(int n) : changed(true), n(n), surface(new Surface*[n]) {
  }

  BlitCached::~BlitCached() {
    for(int i=0; i<n; i++) {
      delete surface[i];
      surface[i] = 0;
    }
    delete[] surface;
  }

  void BlitCached::eachFrame(uint32_t ticks) {
    if(changed) {
      for(int i=0; i<n; i++) {
        delete surface[i];
        surface[i] = render(i);
      }
      changed = false;
    } // if(changed)
  } // eachFrame()

  void BlitCached::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
    for(int i=0; i<n; i++) {
      if(surface[i]) {
        surface[i]->blit(PhysicalPosition(0, 0), target, conv<VirtualPosition, PhysicalPosition>(p));
      }
    }
  }

}

