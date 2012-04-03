// vim: set noexpandtab:

#include "dialog_frontend_speech_bubbles.h"
#include "game.h"

namespace grail {

	DialogFrontendSpeechBubbles::DialogFrontendSpeechBubbles() {
	}

	void DialogFrontendSpeechBubbles::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {

		for (std::vector<DialogText::Ptr>::const_reverse_iterator riter = lines.rbegin();
			riter != lines.rend(); ++riter) {

			if ((*riter)->getDialogLine()->isStarted()) {
				Actor::Ptr actor = (*riter)->getDialogLine()->getSpeaker();

				// render above speaker
				(*riter)->renderAt(target, ticks, (p - Game::getInstance().getViewport().getCameraPosition()) + actor->getUpperLeftCorner());
			}
		}
	}

} // namespace grail
