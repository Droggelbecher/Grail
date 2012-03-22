// vim: set noexpandtab:

#include "dialog_line.h"

namespace grail {

	DialogLine::DialogLine(Actor::Ptr spk, std::string s, uint32_t t) : speaker(spk), text(s), timeToLive(t), started(false), complete(false)  {
	}

	void DialogLine::start() {
		started = true;
		timeStarted = SDL_GetTicks();
	}

	void DialogLine::eachFrame() {
		if (isStarted()) {
			uint32_t timeNow = SDL_GetTicks();

			if (timeNow > (timeStarted + timeToLive)) {
				complete = true;
			}
		}
	}

} //namespace grail

