// vim: set noexpandtab:

#ifndef SPEECH_BUBBLE_H
#define SPEECH_BUBBLE_H

#include "text.h"

namespace grail {

class SpeechBubble {
	Font::Ptr font;
	Text text;
	
	public:
		SpeechBubble(std::string);

		void renderAt(SDL_Surface*, uint32_t, VirtualPosition) const;
};

}

#endif // SPEECH_BUBBLE

