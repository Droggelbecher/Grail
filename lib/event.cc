
#include "event.h"
#include "utils.h"

namespace grail {

void freeUserEventData(SDL_Event& evt) {
  if(evt.type == SDL_USEREVENT) {
    delete (Event*)(evt.user.data1);
    evt.user.data1 = 0;
  }
}

void Event::getData(Event* other) {
  type = other->type;
  position = other->position;
  buttonState = other->buttonState;
  button = other->button;
  keysym = other->keysym;
  actor = other->actor;
}

Event::Event() {
}

Event::Event(const SDL_Event& evt) {
  type = evt.type;
  if(evt.type == SDL_USEREVENT) {
    type = evt.user.code;
    getData((Event*)evt.user.data1);
  }
  else {
    switch(evt.type) {
      case SDL_MOUSEMOTION:
        position = conv<PhysicalPosition, VirtualPosition>(
            PhysicalPosition(evt.motion.x, evt.motion.y)
            );
        buttonState = evt.motion.state;
        break;

      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
        position = conv<PhysicalPosition, VirtualPosition>(
            PhysicalPosition(evt.button.x, evt.button.y)
            );
        button = evt.button.button;
        buttonState = evt.button.state;
        break;

      case SDL_KEYDOWN:
      case SDL_KEYUP:
        keysym = evt.key.keysym;
        break;
    } // switch
  } // else
}

Event* Event::actorClick(Actor::Ptr a, VirtualPosition p, uint8_t b) {
  Event* e = new Event();
  e->type = EVT_ACTOR_CLICK;
  e->actor = a;
  e->position = p;
  e->button = b;
  return e;
}

Event* Event::sceneClick(VirtualPosition p, uint8_t b) {
  Event* e = new Event();
  e->type = EVT_SCENE_CLICK;
  e->position = p;
  e->button = b;
  return e;
}

SDL_Event Event::toSDL() const {
  SDL_Event evt;
  if(type >= _EVT_GRAIL_START) {
    Event* copy = new Event();
    *copy = *this;
    evt.type = SDL_USEREVENT;
    evt.user.code = type;
    evt.user.data1 = (void*)copy;
  }
  else {
    PhysicalPosition pos;
    evt.type = type;
    switch(type) {
      case SDL_MOUSEMOTION:
        pos = conv<VirtualPosition, PhysicalPosition>(position);
        evt.motion.x = pos.getX();
        evt.motion.y = pos.getY();
        evt.motion.state = buttonState;
        break;

      case SDL_MOUSEBUTTONUP:
      case SDL_MOUSEBUTTONDOWN:
        pos = conv<VirtualPosition, PhysicalPosition>(position);
        evt.button.x = pos.getX();
        evt.button.y = pos.getY();
        evt.button.state = buttonState;
        evt.button.button = button;
        break;

      case SDL_KEYDOWN:
      case SDL_KEYUP:
        evt.key.keysym = keysym;
        break;
    } // switch
  } // else

  return evt;
} // toSDL()

void Event::push() const {
  SDL_Event evt = toSDL();
  SDL_PushEvent(&evt);
}

int Event::getType() const { return type; }
Actor::Ptr Event::getActor() const { return actor; }
uint8_t Event::getButton() const { return button; }
VirtualPosition Event::getPosition() const { return position; }
uint8_t Event::getButtonState() const { return buttonState; }
SDL_keysym Event::getKeysym() const { return keysym; }

} // namespace grail


