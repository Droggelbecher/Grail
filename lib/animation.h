
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL/SDL.h>
#include <string>
#include <cassert>

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
    virtual void setDirection(uint16_t direction) { }
    virtual void setDirection(VirtualPosition p) { }
};

class DirectionAnimation : public Animation {
    uint16_t currentDirection;
    uint16_t directions;
    Animation** animations;
  public:
    DirectionAnimation(uint16_t directions);
    ~DirectionAnimation();

    void renderAt(SDL_Surface*, uint32_t, VirtualPosition) const;
    VirtualSize getSize() const;
    bool hasPoint(VirtualPosition) const;
    void eachFrame(uint32_t);

    void setDirection(uint16_t direction) {
      assert(direction < directions);
      currentDirection = direction;
    }

    void setDirection(VirtualPosition p) {
      setDirection(p.nearestDirection(directions));
    }

    void setAnimation(uint16_t direction, Animation* animation);
};

#endif // ANIMATION_H

