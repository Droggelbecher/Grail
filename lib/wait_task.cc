
#include "wait_task.h"

#include <SDL.h>
#include "debug.h"

namespace grail {

	WaitTask::WaitTask(uint32_t duration) : duration(duration) {
	}

	void WaitTask::onStart() {
		expire = SDL_GetTicks() + duration;
	}

	void WaitTask::eachFrame(uint32_t ticks) {
		if(SDL_GetTicks() >= expire) {
			cdbg << "signalling completion! now: " << SDL_GetTicks() << " expire: " << expire << "\n";
			signalComplete();
		}
	}

}

