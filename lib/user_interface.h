// vim: set noexpandtab:

#ifndef USER_INTERFACE_H
#define USER_INTERFACE_H

#include <stdint.h>
#include <string>
#include <list>

#include <SDL.h>
#include <boost/shared_ptr.hpp>

#include "classes.h"
#include "event.h"
#include "user_interface_element.h"
#include "action.h"

namespace grail {
	
	/**
	 * Game user interface.
	 */
	class UserInterface {
			std::list<UserInterfaceElement::Ptr> elements;
			std::list<Actor::Ptr> currentActors;
			bool hovering;
			Action::Ptr currentAction;
			
		public:
			typedef boost::shared_ptr<UserInterface> Ptr;
			
			///
			UserInterface() : hovering(false) { }
			
			///
			virtual ~UserInterface() { }
			
			///
			void addElement(UserInterfaceElement::Ptr element);
			
			///
			void addAnimation(Animation::Ptr animation);
			
			/**
			 * Inform the user interface that the given actor is currently
			 * hovered with the mouse.
			 */
			void setHovering(Actor::Ptr actor);
			
			/// Return currently hovered actor
			Actor::Ptr getHovering() const;
			
			/**
			 * Inform the user interface about the currently chosen action.
			 */
			void setAction(Action::Ptr action) { currentAction = action; }
			
			/// Return currently chosen action
			Action::Ptr getAction() const { return currentAction; }
			
			/// Return text of currently chosen action or the empty string
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

