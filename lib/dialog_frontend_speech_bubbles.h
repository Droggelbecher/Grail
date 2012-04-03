// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_SPEECH_BUBBLES_H
#define DIALOG_FRONTEND_SPEECH_BUBBLES_H

#include "dialog_frontend_text.h"

namespace grail {

	/**
	* Pluggable dialog frontend which displays speech
	* bubbles (lines of text) above actors' heads
	*/
	class DialogFrontendSpeechBubbles : public DialogFrontendText {

		public:
			DialogFrontendSpeechBubbles();

			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
	};

} // namespace grail

#endif //DIALOG_FRONTEND_SPEECH_BUBBLES_H

