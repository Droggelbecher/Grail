// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {
		subtitlePosition = VirtualPosition(100, 100);
	}

	void DialogFrontendSubtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) {

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

			(*iter).second->eachFrame(ticks); // temporarily here

			
		}
	}
} // namespace grail

