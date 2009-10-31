
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>
#include <iostream>

#include "vector2d.h"
#include "animation.h"
#include "registry.h"

class Actor : public Registrable {
    Animation *animation;
    std::map<std::string, Animation*> animationModes;
    std::string mode;

    VirtualPosition position;
  public:

    struct CompareByY {
      bool operator()(Actor* a, Actor* b) const {
        return a->position.getY() < b->position.getY();
      }
    };


    Actor() : Registrable("Actor"), animation(0) { }

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      std::cerr << "rendering actor" << std::endl;
      if(animation) {
        std::cerr << "rendering at " << p.getX() << " " << p.getY() << std::endl;
        animation->renderAt(target, ticks, p + position);
      }
    }

    void addAnimation(std::string mode, Animation& animation) {
      animationModes[mode] = &animation;
    }

    void setMode(std::string mode) {
      this->mode = mode;
      if(animationModes.count(mode)) {
        animation = animationModes[mode];
      }
    }

    void setPosition(VirtualPosition position) {
      position = position;
    }
};

#endif // SCENE_ENTITY_H

