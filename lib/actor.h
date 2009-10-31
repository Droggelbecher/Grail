
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include <map>
#include <string>

#include "vector2d.h"
#include "animation.h"
#include "registry.h"

class Actor : public Registrable {
    const Animation *animation;
    std::map<std::string, Animation*> animationModes;

  public:
    VirtualPosition position;

    Actor() : Registrable("Actor"), animation(0) { }

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      if(animation) {
        animation->renderAt(target, ticks, p);
      }
    }
};

#endif // SCENE_ENTITY_H

