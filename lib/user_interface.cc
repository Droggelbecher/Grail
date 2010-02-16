
#include "user_interface.h"

namespace grail {

  EventState UserInterface::handleEvent(SDL_Event& event, uint32_t frameDuration) {
    return EVENT_STATE_UNHANDLED;
  }

  void UserInterface::addElement(UserInterfaceElement::Ptr element) {
    elements.push_back(element);
  }

  void UserInterface::eachFrame(uint32_t ticks) {
    std::list<UserInterfaceElement::Ptr>::iterator iter;
    for(iter = elements.begin(); iter != elements.end(); iter++) {
      (*iter)->eachFrame(ticks);
    }
  }

  void UserInterface::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
    std::list<UserInterfaceElement::Ptr>::const_iterator iter;
    for(iter = elements.begin(); iter != elements.end(); iter++) {
      (*iter)->renderAt(target, ticks, p);
    }
  }

  std::ostream& operator<<(std::ostream& os, const UserInterface& ui) {
    os << "UserInterface";
    return os;
  }
} // namespace grail

