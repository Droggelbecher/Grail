
#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdint.h>
#include <string>

#include <SDL/SDL.h>
#include <boost/shared_ptr.hpp>

#include "classes.h"
#include "event.h"

namespace grail {

class UserInterface {
  public:
    typedef boost::shared_ptr<UserInterface> Ptr;

    static const std::string className;

    UserInterface() { }
    virtual ~UserInterface() { }

    virtual EventState handleEvent(SDL_Event& event, uint32_t frameDuration);
};

} // namespace grail

#endif // USER_INTERFACE_H

