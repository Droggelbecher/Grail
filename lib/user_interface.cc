// vim: set noexpandtab:

#include "user_interface.h"
#include "user_interface_animation.h"
#include "debug.h"

namespace grail {
	
	EventState UserInterface::handleEvent(const SDL_Event& event, uint32_t frameDuration) {
		std::list<UserInterfaceElement::Ptr>::const_iterator iter;
		for(iter = elements.begin(); iter != elements.end(); iter++) {
			if((*iter)->handleEvent(event, frameDuration) == EVENT_STATE_HANDLED) {
				return EVENT_STATE_HANDLED;
			}
		}
		return EVENT_STATE_UNHANDLED;
	}
	
	void UserInterface::addElement(UserInterfaceElement::Ptr element) {
		elements.push_back(element);
	}
	
	void UserInterface::addAnimation(Animation::Ptr animation) {
		UserInterfaceElement::Ptr elem(new UserInterfaceAnimation(animation));
		addElement(elem);
	}
	
	void UserInterface::setHovering(Actor::Ptr actor) {
		if(hovering) {
			currentActors.pop_back();
		}
		if(actor) {
			hovering = true;
			currentActors.push_back(actor);
		}
		else {
			hovering = false;
		}
	}
	
	Actor::Ptr UserInterface::getHovering() const {
		if(hovering) {
			return currentActors.back();
		}
		return Actor::Ptr();
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

