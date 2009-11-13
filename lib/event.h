
#ifndef EVENT_H
#define EVENT_H

#include <SDL/SDL.h>
#include "vector2d.h"
#include "actor.h"

enum EventState { EVENT_STATE_HANDLED, EVENT_STATE_UNHANDLED };

enum EventType { _EVT_GRAIL_START = 100, EVT_SCENE_CLICK = 100, EVT_ACTOR_CLICK };

struct Event {
  virtual ~Event() { }

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
    evt.user.code = getType();
    evt.user.data1 = (Event*)this;
    SDL_PushEvent(&evt);
  }

  static Event* fromSDL(const SDL_Event& evt);

  virtual int getType() const = 0;
  virtual Actor* getActor() const { return 0; }
  virtual uint8_t getButton() const { return 0; }
  virtual VirtualPosition getPosition() const { return VirtualPosition(0, 0); }

};

void freeUserEventData(SDL_Event& evt);


struct SceneClickEvent : public Event {
  VirtualPosition position;

  SceneClickEvent(VirtualPosition position) : position(position) { }

  int getType() const { return EVT_SCENE_CLICK; }
  VirtualPosition getPosition() const { return position; }
};

struct ActorClickEvent : public Event {
  VirtualPosition position;
  Actor& actor;
  uint8_t button;

  ActorClickEvent(Actor& actor, VirtualPosition position, uint8_t button) :
    position(position), actor(actor), button(button) { }

  int getType() const { return EVT_ACTOR_CLICK; }
  VirtualPosition getPosition() const { return position; }
  Actor* getActor() const { return &actor; }
  uint8_t getButton() const { return button; }
};

// ----

struct OmniEvent {
  private:
    const SDL_Event& evt;

  public:
    OmniEvent(const SDL_Event& evt);

    int getType() const;
    VirtualPosition getPosition() const;
    Actor* getActor() const;
    uint8_t getButton() const;
};


#endif // EVENT_H

