
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
#include "area.h"

class Scene : public Object {
    const Animation* background;
    list<Actor*> actors;
    Area *area;

  public:
    Scene();
    virtual ~Scene();

    VirtualSize getSize() {
      if(background) { return background->getSize(); }
      return VirtualSize(0, 0);
    }

    void setBackground(const Animation& background) {
      this->background = &background;
    }

    void addActor(Actor& entity) {
      actors.push_back(&entity);
      inplace_merge(actors.begin(), actors.end(), actors.end(), Actor::CompareByY());
    }

    void eachFrame(uint32_t ticks) {
      list<Actor*>::const_iterator iter;
      for(iter = actors.begin(); iter != actors.end(); iter++) {
        (*iter)->eachFrame(ticks);
      }
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

    EventState handleEvent(SDL_Event& event, uint32_t ticks) {
      if(event.type == SDL_MOUSEBUTTONDOWN) {
        VirtualPosition pos = conv<SDL_MouseButtonEvent&, VirtualPosition>(event.button);

        if(area->hasPoint(pos)) {
          list<Actor*>::const_iterator iter;
          bool event_sent = false;
          for(iter = actors.begin(); iter != actors.end(); iter++) {
            if((*iter)->hasPoint(pos)) {
              (new ActorClickEvent(**iter, pos, event.button.button))->push();
              event_sent = true;
              break;
            }
          } // for
          if(!event_sent) {
            (new SceneClickEvent(pos))->push();
          }
        } // if area has point
      } // if mouse button down

      return EVENT_STATE_UNHANDLED;
    }
};

#endif // SCENE_H

