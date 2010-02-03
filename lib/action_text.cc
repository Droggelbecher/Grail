
#include "action_text.h"
#include "font.h"

namespace grail {

  ActionText::ActionText(Font::Ptr fnt) : text(fnt) {
    text.setText("Go to blah");
  }

  EventState ActionText::handleEvent(SDL_Event& event, uint32_t frameDuration) {
    return EVENT_STATE_UNHANDLED;
  }

}

