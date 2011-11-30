// vim: set noexpandtab:

#include "speech_bubble.h"

namespace grail {

SpeechBubble::SpeechBubble(std::string t) : font(new Font("fonts/tommy_holloway.ttf", 40, 1)), text(font) {

}

void SpeechBubble::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
	text.renderAt(target, ticks, p);
}

}

