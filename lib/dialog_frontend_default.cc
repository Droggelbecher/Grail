// vim: set noexpandtab:

#include "dialog_frontend_default.h"

namespace grail {

	void DialogFrontendDefault::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) {

		// remove unused text objects
		for (std::map<Actor::Ptr, Text::Ptr>::iterator iter = lines.begin();
			iter != lines.end(); iter++) {
			if (!(*iter).first->isSpeaking()) {
				lines.erase(iter);
			}
		}

		// render the text objects
		for (std::map<Actor::Ptr, Text::Ptr>::iterator iter = lines.begin();
			iter != lines.end(); iter++) {

			VirtualPosition actorPosition = (*iter).first->getPosition();
			(*iter).second->eachFrame(ticks); // temporarily here
			(*iter).second->renderAt(target, ticks, actorPosition);
		}
	}
} // namespace grail

