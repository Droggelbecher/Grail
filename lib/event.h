
#ifndef EVENT_H
#define EVENT_H

#include <SDL/SDL.h>
#include "vector2d.h"
#include "actor.h"

enum EventState { EVENT_STATE_HANDLED, EVENT_STATE_UNHANDLED };

enum EventType { EVT_SCENE_CLICK, EVT_ACTOR_CLICK };

struct Event {
  virtual ~Event() { }
  virtual int getCode() const = 0;

  /**
   * Push a pointer to this event to the SDL event queue.
   * Note that this should be really on the heap while doing so,
   * else when polling the event back its memory might already been freed.
   *
   * Free it after polling with:
   *
   *  delete (Event*)(sdl_event.user.data1) after
   *
   * after making sure it is a SDL_USEREVENT.
   */
  void push() const {
    SDL_Event evt;
    evt.type = SDL_USEREVENT;
    evt.user.code = getCode();
    evt.user.data1 = (Event*)this;
    SDL_PushEvent(&evt);
  }

  static Event* fromSDL(const SDL_Event& evt);
};

void freeUserEventData(SDL_Event& evt);


struct SceneClickEvent : public Event {
  int getCode() const { return EVT_SCENE_CLICK; }
  VirtualPosition position;

  SceneClickEvent(VirtualPosition position) : position(position) { }
};

struct ActorClickEvent : public Event {
  int getCode() const { return EVT_ACTOR_CLICK; }
  Actor& actor;
  VirtualPosition position;

  ActorClickEvent(Actor& actor, VirtualPosition position) : actor(actor), position(position) { }
};

#endif // EVENT_H

