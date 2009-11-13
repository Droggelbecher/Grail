
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <SDL/SDL.h>
#include <stdint.h>
#include "classes.h"
#include "game.h"
#include "registry.h"

class UserInterface : public Object {
  public:
    UserInterface() : Object("UserInterface") { }
    virtual EventState handleEvent(SDL_Event& event, uint32_t frameDuration);
};

#endif // USER_INTERFACE_H

