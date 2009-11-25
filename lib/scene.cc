
#include "scene.h"

#include "viewport.h"
#include "rect.h"

#include <cassert>
#include <list>
using std::list;
#include <iostream>
using std::cerr;
using std::endl;

Scene::Scene() : Object("Scene"), background(0) {
}

Scene::~Scene() {
}

VirtualSize Scene::getSize() const {
  if(background) { return background->getSize(); }
  return VirtualSize(0, 0);
}

void Scene::eachFrame(uint32_t ticks) {
  list<Actor*>::const_iterator iter;
  for(iter = actors.begin(); iter != actors.end(); iter++) {
    (*iter)->eachFrame(ticks);
  }
}

void Scene::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
  assert(target);
  if(background) {
    background->renderAt(target, ticks, p);
  }

  list<Actor*>::const_iterator iter;
  for(iter = actors.begin(); iter != actors.end(); iter++) {
    (*iter)->renderAt(target, ticks, p);
  }

} // renderAt

EventState Scene::handleEvent(SDL_Event& event, uint32_t ticks) {
  if(event.type == SDL_MOUSEBUTTONDOWN) {
    VirtualPosition pos = conv<SDL_MouseButtonEvent&, VirtualPosition>(event.button);

    if(Rect(getSize()).hasPoint(pos)) {
      list<Actor*>::const_iterator iter;
      bool event_sent = false;
      VirtualPosition cam = Game::getInstance().getViewport().getCameraPosition();

      for(iter = actors.begin(); iter != actors.end(); iter++) {
        if((*iter)->hasPoint(pos)) {
          (new ActorClickEvent(**iter, pos + cam, event.button.button))->push();
          event_sent = true;
          break;
        }
      } // for
      if(!event_sent) {
        (new SceneClickEvent(pos + cam))->push();
      }
    } // if area has point
  } // if mouse button down

  return EVENT_STATE_UNHANDLED;
}

