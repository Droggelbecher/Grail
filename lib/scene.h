
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
    list<Actor*> entities;

    struct CompareByY {
      bool operator()(Actor* a, Actor* b) const {
        return a->position.y() < b->position.y();
      }
    };

  public:
    Scene() : Registrable("Scene"), background(NULL) { }
    ~Scene() {
      delete background;
    }

    void setBackground(const Animation& background) {
      this->background = &background;
    }

    void addEntity(Actor& entity) {
      entities.push_back(&entity);
      inplace_merge(entities.begin(), entities.end(), entities.end(), CompareByY());
    }

    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      assert(target != NULL);
      if(background != NULL) {
        background->renderAt(target, ticks, p);
      }
    } // renderAt

    EventState handleEvent(Event& event, uint32_t ticks) {
      cerr << "scene::handleEvent" << endl;
      return EVENT_STATE_UNHANDLED;
    }
};

#endif // SCENE_H

