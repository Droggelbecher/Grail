
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
#include "action.h"

namespace grail {

  class UserInterface {
      std::list<UserInterfaceElement::Ptr> elements;
      std::list<Actor::Ptr> currentActors;
      bool hovering;
      Action::Ptr currentAction;

    public:
      typedef boost::shared_ptr<UserInterface> Ptr;

      UserInterface() : hovering(false) { }
      virtual ~UserInterface() { }

      void addElement(UserInterfaceElement::Ptr element);
      void addAnimation(Animation::Ptr animation);

      void setHovering(Actor::Ptr actor);
      Actor::Ptr getHovering() const;

      void setAction(Action::Ptr action) { currentAction = action; }
      Action::Ptr getAction() const { return currentAction; }

      std::string getActionText() const {
        if(currentAction) {
          return currentAction->getWording(currentActors);
        }
        return "";
      }

      virtual void eachFrame(uint32_t ticks);
      virtual EventState handleEvent(const SDL_Event& event, uint32_t frameDuration);
      virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
  };

  std::ostream& operator<<(std::ostream& os, const UserInterface& ui);

} // namespace grail

#endif // USER_INTERFACE_H

