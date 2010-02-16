
#ifndef ANIMATION_H
#define ANIMATION_H

#include <string>
#include <cassert>

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "area.h"

namespace grail {

class Animation : public Area {

  public:
    typedef boost::shared_ptr<Animation> Ptr;
    typedef boost::shared_ptr<const Animation> ConstPtr;

    Animation() { }
    virtual ~Animation() { }

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
    virtual VirtualSize getSize() const = 0;
    virtual bool hasPoint(VirtualPosition p) const {
      return (p.getX() >= 0) && (p.getX() < getSize().getX()) &&
        (p.getY() >= 0) && (p.getY() < getSize().getY());
    }
    virtual void eachFrame(uint32_t ticks) { }
    virtual uint16_t getDirection() const { return 0; }
    virtual void setDirection(uint16_t direction) { }
    virtual void setDirection(VirtualPosition p) { }

    virtual void makeContinuationOf(const Animation& other) {
      setDirection(other.getDirection());
    }
};

class DirectionAnimation : public Animation {
    uint16_t currentDirection;
    uint16_t directions;
    double offset;
    Animation::Ptr* animations;
  public:
    static const std::string className;

    DirectionAnimation(uint16_t directions, double offset=0);
    ~DirectionAnimation();

    void renderAt(SDL_Surface*, uint32_t, VirtualPosition) const;
    VirtualSize getSize() const;
    bool hasPoint(VirtualPosition) const;
    void eachFrame(uint32_t);


    uint16_t getDirection() const { return currentDirection; }

    void setDirection(uint16_t direction) {
      assert(direction < directions);
      currentDirection = direction;
    }

    void setDirection(VirtualPosition p) {
      setDirection(p.nearestDirection(directions, offset));
    }

    void setAnimation(uint16_t direction, Animation::Ptr animation);
};

} // namespace grail

#endif // ANIMATION_H

