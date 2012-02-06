// vim: set noexpandtab:

#include "dialog_line.h"

namespace grail {

	DialogLine::DialogLine(std::string s, uint32_t t) : text(s), timeToLive(t), started(false), complete(false)  {
	}

	void DialogLine::start() {

		started = true;
		timeStarted = SDL_GetTicks();

		printf("activated\n");
	}

	bool DialogLine::isStarted() {

		return started;
	}

	void DialogLine::eachFrame() {

		if (isStarted()) {
			uint32_t timeNow = SDL_GetTicks();

			if (timeNow > (timeStarted + timeToLive)) {
				complete = true;
			}
		}
	}

	bool DialogLine::isComplete() {
		return complete;
	}

} //namespace grail

