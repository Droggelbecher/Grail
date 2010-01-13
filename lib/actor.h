
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>
#include <iostream>
#include <list>

#include "classes.h"
#include "vector2d.h"
#include "animation.h"
#include "area.h"
#include "reference_counting.h"

#include <iostream>

namespace grail {

typedef std::list<VirtualPosition> Path;

class Actor : public Area {
    Reference<Animation> animation;
    std::map<std::string, Animation*> animationModes;
    std::string mode;

    VirtualPosition position;
    double alignmentX, alignmentY;
    Reference<Area> area;
    double speed; ///< Unit is virtual pixels / second

    Path walkPath;

  protected:
    VirtualPosition getUpperLeftCorner() const;

  public:
    static const std::string className;

    struct CompareByY {
      bool operator()(Actor* a, Actor* b) const {
        return a->position.getY() < b->position.getY();
      }
    };

    Actor() : mode("default"), alignmentX(0.5), alignmentY(1.0), speed(1000.0)
    {
    }

    bool hasPoint(VirtualPosition p) const {
      if(area) {
        return area->hasPoint(p - getUpperLeftCorner());
      }
      else if(animation) {
        return animation->hasPoint(p - getUpperLeftCorner());
      }
      else {
        return false;
      }
    }

    void eachFrame(uint32_t ticks);

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      if(animation) {
        animation->renderAt(target, ticks, getUpperLeftCorner() + p);
      }
    }

    void addAnimation(std::string mode, Animation& animation) {
      animationModes[mode] = &animation;
      if(animationModes.count(mode)) {
        this->animation = animationModes[mode];
      }
    }

    void setAlignment(double x, double y);

    void setMode(std::string mode) {
      this->mode = mode;
      if(animationModes.count(mode)) {
        animation = animationModes[mode];
      }
    }

    void setPosition(VirtualPosition position) { this->position = position; }
    VirtualPosition getPosition() { return position; }

    void walk(const Path& path);
    void walkStraight(VirtualPosition p);
};

} // namespace grail

#endif // ACTOR_H

