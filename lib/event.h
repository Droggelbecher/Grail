
#ifndef EVENT_H
#define EVENT_H

#include <SDL/SDL.h>
#include "vector2d.h"
#include "actor.h"

enum EventState { EVENT_STATE_HANDLED, EVENT_STATE_UNHANDLED };

struct Event {
  enum Type {
    EVT_SCENECLICK = SDL_NUMEVENTS + 100,
    EVT_ACTORCLICK,
  };

  int type;
};

struct SDLEvent : public Event {
  SDL_Event& sdl;
  SDLEvent(SDL_Event& sdl) : sdl(sdl) {
    type = sdl.type;
  }
};

struct SceneClickEvent : public Event {
  VirtualPosition position;
};

struct ActorClickEvent : public Event {
  VirtualPosition position;
  Actor& actor;
};



#endif // EVENT_H

