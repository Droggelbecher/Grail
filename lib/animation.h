
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL/SDL.h>
#include <string>

#include "vector2d.h"
#include "registry.h"

class Animation : public Registrable {
  public:
    Animation(std::string classname) : Registrable(classname) { }
    virtual ~Animation() { }

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const = 0;
};

#endif // ANIMATION_H

