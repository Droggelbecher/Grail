
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

  public:
    typedef boost::shared_ptr<UserInterface> Ptr;
    typedef boost::shared_ptr<const UserInterface> ConstPtr;

    UserInterface() { }
    virtual ~UserInterface() { }

    void addElement(UserInterfaceElement::Ptr element);
    virtual void eachFrame(uint32_t ticks);
    virtual EventState handleEvent(SDL_Event& event, uint32_t frameDuration);
    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
};

} // namespace grail

#endif // USER_INTERFACE_H

