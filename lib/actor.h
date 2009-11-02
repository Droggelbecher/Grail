
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>
#include <iostream>

#include "vector2d.h"
#include "animation.h"
#include "registry.h"
#include "area.h"



#include <iostream>
using namespace std;

class Actor : public Registrable, public Area {
    Animation *animation;
    std::map<std::string, Animation*> animationModes;
    std::string mode;

    VirtualPosition position;
    double alignmentX, alignmentY;
    Area* area;

  protected:
    VirtualPosition getUpperLeftCorner() const;

  public:

    struct CompareByY {
      bool operator()(Actor* a, Actor* b) const {
        return a->position.getY() < b->position.getY();
      }
    };


    Actor() : Registrable("Actor"), animation(0), alignmentX(0.5), alignmentY(1.0), area(0) { }

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

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      if(animation) {
        animation->renderAt(target, ticks, getUpperLeftCorner() + p);
      }
    }

    void addAnimation(std::string mode, Animation& animation) {
      animationModes[mode] = &animation;
    }

    void setAlignment(double x, double y);

    void setMode(std::string mode) {
      this->mode = mode;
      if(animationModes.count(mode)) {
        animation = animationModes[mode];
      }
    }

    void setPosition(VirtualPosition position) {
      this->position = position;
    }
};

#endif // SCENE_ENTITY_H

