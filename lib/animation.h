
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL/SDL.h>

#include "vector2d.h"

class Animation {
  public:
    Animation() { }
    virtual ~Animation() { }

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const = 0;
};

#endif // ANIMATION_H

