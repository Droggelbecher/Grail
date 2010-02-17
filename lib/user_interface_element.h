
#ifndef USER_INTERFACE_ELEMENT_H
#define USER_INTERFACE_ELEMENT_H

#include <boost/shared_ptr.hpp>
#include <SDL/SDL.h>
#include "vector2d.h"
#include "event.h"

namespace grail {

  class UserInterfaceElement {
      double alignmentX, alignmentY;
      VirtualPosition position;

    public:
      typedef boost::shared_ptr<UserInterfaceElement> Ptr;

      UserInterfaceElement();
      virtual ~UserInterfaceElement() { };

      VirtualPosition getUpperLeftCorner() const;
      VirtualPosition getPosition() const { return position; }
      void setPosition(VirtualPosition p) { position = p; }

      double getAlignmentX() { return alignmentX; }
      void setAlignmentX(double x) { alignmentX = x; }

      double getAlignmentY() { return alignmentY; }
      void setAlignmentY(double y) { alignmentY = y; }

      virtual VirtualPosition getSize() const = 0;

      virtual void eachFrame(uint32_t ticks) = 0;
      virtual EventState handleEvent(const SDL_Event& event, uint32_t frameDuration) = 0;
      virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
  };

} // namespace grail

#endif // USER_INTERFACE_ELEMENT_H

