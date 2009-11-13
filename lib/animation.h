
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL/SDL.h>
#include <string>

#include "vector2d.h"
#include "registry.h"
#include "area.h"

class Animation : public Object, public Area {

  public:
    Animation(std::string classname) : Object(classname) { }
    virtual ~Animation() { }

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
    virtual VirtualSize getSize() const = 0;
    virtual bool hasPoint(VirtualPosition p) const {
      return (p.getX() >= 0) && (p.getX() < getSize().getX()) &&
        (p.getY() >= 0) && (p.getY() < getSize().getY());
    }
    virtual void eachFrame(uint32_t ticks) { }
};

#endif // ANIMATION_H

