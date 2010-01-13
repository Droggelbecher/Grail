
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <SDL/SDL.h>
#include <stdint.h>
#include <string>
#include "classes.h"
#include "game.h"
#include "reference_counting.h"

class UserInterface : public ReferenceCounted {
  public:
    static const std::string className;

    UserInterface() { }
    virtual EventState handleEvent(SDL_Event& event, uint32_t frameDuration);
};

#endif // USER_INTERFACE_H

