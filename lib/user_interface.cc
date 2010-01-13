
#include "user_interface.h"

const std::string UserInterface::className = "UserInterface";

EventState UserInterface::handleEvent(SDL_Event& event, uint32_t frameDuration) {
  return EVENT_STATE_UNHANDLED;
}

