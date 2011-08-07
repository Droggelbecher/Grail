// vim: set noexpandtab:

#include "action_text.h"
#include "font.h"

namespace grail {
	
	ActionText::ActionText(Font::Ptr fnt) : text(fnt) {
		// TODO
		text.setText("Go to blah");
	}
	
	EventState ActionText::handleEvent(const SDL_Event& event, uint32_t frameDuration) {
		return EVENT_STATE_UNHANDLED;
	}
	
}

