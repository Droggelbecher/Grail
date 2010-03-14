// vim: set noexpandtab:

#ifndef USER_INTERFACE_ANIMATION_H
#define USER_INTERFACE_ANIMATION_H

#include "user_interface_element.h"
#include "animation.h"

namespace grail {
	
	class UserInterfaceAnimation : public UserInterfaceElement {
		private:
			Animation::Ptr animation;
			
		public:
			UserInterfaceAnimation(Animation::Ptr animation) : animation(animation) {
			}
			
			VirtualSize getSize() const { return animation->getSize(); }
			
			void eachFrame(uint32_t ticks) {
				animation->eachFrame(ticks);
			}
			
			EventState handleEvent(const SDL_Event& event, uint32_t frameDuration) {
				return EVENT_STATE_UNHANDLED;
			}
			
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
				animation->renderAt(target, ticks, getUpperLeftCorner() + p);
			}
	};
	
}

#endif // USER_INTERFACE_ANIMATION_H

