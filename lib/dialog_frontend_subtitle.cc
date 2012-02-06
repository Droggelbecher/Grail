// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		int defaultX = 1300;
		int defaultY = 2800;
		subtitlePosition = VirtualPosition(defaultX, defaultY);
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

			// set the actor's text object to the text of what they are saying
			(*iter).second->setText((*iter).first->getDialogLine()->getText());

			(*iter).second->eachFrame(ticks); // temporarily here

			// render the subtitle
			(*iter).second->renderAt(target, ticks, subtitlePosition);
		}
	}
} // namespace grail

