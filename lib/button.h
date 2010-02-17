
#ifndef BUTTON_H
#define BUTTON_H

#include "user_interface_element.h"
#include "animation.h"
#include "vector2d.h"

namespace grail {

  class Button : public UserInterfaceElement {
    public:
      typedef EventState (*Callback)(Button&, const SDL_Event&, uint32_t);

    private:
      Animation::Ptr animation;
      Callback onClick;

    public:

      Button(Animation::Ptr animation, Callback onClick) : animation(animation), onClick(onClick) {
      }

      VirtualPosition getSize() const { return animation->getSize(); }

      void eachFrame(uint32_t ticks) { }

      EventState handleEvent(const SDL_Event& event, uint32_t frameDuration) {

        if(event.type == SDL_MOUSEBUTTONDOWN) {
          VirtualPosition clickPosition = conv<const SDL_MouseButtonEvent&, VirtualPosition>(event.button);
          if(animation->hasPoint(clickPosition) && onClick) {
            return onClick(*this, event, frameDuration);
          }
        }

        return EVENT_STATE_UNHANDLED;
      }

      void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
        animation->renderAt(target, ticks, getUpperLeftCorner() + p);
      }

  };

}

#endif // BUTTON_H

