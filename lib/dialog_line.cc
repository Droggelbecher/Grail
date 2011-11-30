// vim: set noexpandtab:

#include "dialog_line.h"

namespace grail {

	DialogLine::DialogLine(std::string s, uint32_t t) : text(s), timeToLive(t), complete(false)  {
		timeStarted = SDL_GetTicks();
	}

	void DialogLine::eachFrame() {
		uint32_t timeNow = SDL_GetTicks();
		if (timeNow > (timeStarted + timeToLive))
		{
			complete = true;
		}
	}

	bool DialogLine::isComplete() {
		return complete;
	}

} //namespace grail

