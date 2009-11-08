
#include "event.h"
#include "utils.h"

void freeUserEventData(SDL_Event& evt) {
  if(evt.type == SDL_USEREVENT) {
    delete (Event*)(evt.user.data1);
    evt.user.data1 = 0;
  }
}


OmniEvent::OmniEvent(SDL_Event& evt) : evt(evt) {
}

int OmniEvent::getType() const {
  if(evt.type == SDL_USEREVENT) { return evt.user.code; }
  return evt.type;
}

VirtualPosition OmniEvent::getPosition() const {
  switch(getType()) {
    case EVT_SCENE_CLICK:
      return ((SceneClickEvent*)evt.user.data1)->position;
      break;
    case EVT_ACTOR_CLICK:
      return ((ActorClickEvent*)evt.user.data1)->position;
      break;
    case SDL_MOUSEMOTION:
      return conv<PhysicalPosition, VirtualPosition>(
          PhysicalPosition(evt.motion.x, evt.motion.y)
          );
      break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      return conv<PhysicalPosition, VirtualPosition>(
          PhysicalPosition(evt.button.x, evt.button.y)
          );
      break;
    default:
      //throw Exception("Don't know how to get position for this event");
      return VirtualPosition(0, 0);
      break;
  }
}

Actor* OmniEvent::getActor() {
  switch(getType()) {
    case EVT_ACTOR_CLICK:
      return &( ((ActorClickEvent*)evt.user.data1)->actor );
      break;
    default:
      //throw Exception("Don't know how to get actor for this event");
      return 0;
      break;
  }
}


