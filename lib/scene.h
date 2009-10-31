
#ifndef SCENE_H
#define SCENE_H

#include <cassert>
#include <iostream>
using std::cerr;
using std::endl;
#include <algorithm>
#include <list>
using std::list;

#include "classes.h"
#include "game.h"
#include "vector2d.h"
#include "registry.h"
#include "animation.h"
#include "actor.h"

class Scene : public Registrable {
    const Animation* background;
    list<Actor*> actors;

  public:
    Scene() : Registrable("Scene"), background(NULL) { }
    virtual ~Scene() {
    }

    void setBackground(const Animation& background) {
      this->background = &background;
    }

    void addActor(Actor& entity) {
      actors.push_back(&entity);
      inplace_merge(actors.begin(), actors.end(), actors.end(), Actor::CompareByY());
    }

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      assert(target != NULL);
      if(background != NULL) {
        background->renderAt(target, ticks, p);
      }

      list<Actor*>::const_iterator iter;
      for(iter = actors.begin(); iter != actors.end(); iter++) {
        (*iter)->renderAt(target, ticks, p);
      }

    } // renderAt

    EventState handleEvent(Event& event, uint32_t ticks) {
      return EVENT_STATE_UNHANDLED;
    }
};

#endif // SCENE_H

