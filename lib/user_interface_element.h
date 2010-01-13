
#ifndef USER_INTERFACE_ELEMENT_H
#define USER_INTERFACE_ELEMENT_H

#include "game.h"

namespace grail {

class UserInterfaceElement {
  public:
    UserInterfaceElement() { };
    virtual ~UserInterfaceElement() { };

    virtual EventState handleEvent(Event& event, uint32_t frameDuration) = 0;
};

} // namespace grail

#endif // USER_INTERFACE_ELEMENT_H

