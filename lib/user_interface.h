
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdint.h>
#include <string>
#include <list>

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

#include "classes.h"
#include "event.h"
#include "user_interface_element.h"

namespace grail {

class UserInterface {
    std::list<UserInterfaceElement::Ptr> elements;
    Actor::Ptr hovering;

  public:
    typedef boost::shared_ptr<UserInterface> Ptr;

    UserInterface() { }
    virtual ~UserInterface() { }

    void addElement(UserInterfaceElement::Ptr element);
    void addAnimation(Animation::Ptr animation);

    void setHovering(Actor::Ptr hovering) { this->hovering = hovering; }
    Actor::Ptr getHovering() { return hovering; }

    virtual void eachFrame(uint32_t ticks);
    virtual EventState handleEvent(const SDL_Event& event, uint32_t frameDuration);
    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
};

std::ostream& operator<<(std::ostream& os, const UserInterface& ui);

} // namespace grail

#endif // USER_INTERFACE_H

