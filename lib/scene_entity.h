
#ifndef SCENE_ENTITY_H
#define SCENE_ENTITY_H

#include "vector2d.h"
#include "animation.h"
#include "registry.h"

class SceneEntity : public Registrable {
    const Animation *animation;
  public:
    SceneEntity() : Registrable("SceneEntity"), animation(NULL) { }
    void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const {
    }
};

#endif // SCENE_ENTITY_H

