
#ifndef EVENT_H
#define EVENT_H

#include <SDL/SDL.h>
#include "vector2d.h"
#include "actor.h"

namespace grail {

enum EventState { EVENT_STATE_HANDLED, EVENT_STATE_UNHANDLED };
enum EventType { _EVT_GRAIL_START = 100, EVT_SCENE_CLICK = 100, EVT_ACTOR_CLICK };

class Event;
class UserEvent;

class Event {
  private:
    uint8_t type;
    VirtualPosition position;
    uint8_t buttonState;
    uint8_t button;
    SDL_keysym keysym;
    Actor* actor;

  public:
    Event();
    Event(const SDL_Event& evt);

    virtual ~Event() { }

    SDL_Event toSDL() const;
    void push() const;

    int getType() const;
    Actor* getActor() const;
    uint8_t getButton() const;
    VirtualPosition getPosition() const;
    uint8_t getButtonState() const;
    SDL_keysym getKeysym() const;

    static Event* actorClick(Actor* a, VirtualPosition, uint8_t button);

};

} // namespace grail

#endif // EVENT_H

