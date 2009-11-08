
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
      if(getType() >= _EVT_GRAIL_START) {
        return ((Event*)evt.user.data1)->getPosition();
      }
      else {
        return VirtualPosition(0, 0);
      }
      break;
  }
}

Actor* OmniEvent::getActor() const {
  if(getType() >= _EVT_GRAIL_START) {
    return ((Event*)evt.user.data1)->getActor();
  }
  return 0;
}

uint8_t OmniEvent::getButton() const {
  switch(getType()) {
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
      return evt.button.button;
      break;

    default:
      if(getType() >= _EVT_GRAIL_START) {
        return ((Event*)evt.user.data1)->getButton();
      }
      break;
  }
  return 0;
}



