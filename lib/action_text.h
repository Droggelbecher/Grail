
#ifndef ACTION_TEXT_H
#define ACTION_TEXT_H

#include <SDL/SDL.h>
#include "user_interface_element.h"
#include "vector2d.h"
#include "text.h"
#include "game.h"
#include "actor.h"
#include "debug.h"

namespace grail {

  class ActionText : public UserInterfaceElement {
      Text text;

    public:
      ActionText(Font::Ptr font);
      virtual ~ActionText() { };

      EventState handleEvent(const SDL_Event& event, uint32_t frameDuration);

      void eachFrame(uint32_t ticks) {
        std::string s = Game::getInstance().getUserInterface()->getActionText();
        text.setText(s);
        text.eachFrame(ticks);
      }

      void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
        text.renderAt(target, ticks, p + getUpperLeftCorner());
      }

      void setOutlineFont(Font::Ptr f, int outline) {
        text.setOutlineFont(f, outline);
      }

      VirtualPosition getSize() const {
        return text.getSize();
      }
  };

}

#endif // ACTION_TEXT_ELEMENT_H

