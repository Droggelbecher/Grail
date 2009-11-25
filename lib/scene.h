
#ifndef SCENE_H
#define SCENE_H

#include <algorithm>

#include "classes.h"
#include "game.h"
#include "vector2d.h"
#include "registry.h"
#include "animation.h"
#include "actor.h"

class Scene : public Object {
    const Animation* background;
    std::list<Actor*> actors;

  public:
    Scene();
    virtual ~Scene();

    VirtualSize getSize() const;

    void setBackground(const Animation& background) { this->background = &background; }

    void addActor(Actor& entity) {
      actors.push_back(&entity);
      std::inplace_merge(actors.begin(), actors.end(), actors.end(), Actor::CompareByY());
    }

    void eachFrame(uint32_t ticks);
    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;

    EventState handleEvent(SDL_Event& event, uint32_t ticks);
};

#endif // SCENE_H

