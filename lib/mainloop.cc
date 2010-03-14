// vim: set noexpandtab:

#include <iostream>
#include <algorithm>
#include <SDL.h>

#include "mainloop.h"
#include "game.h"
#include "utils.h"
#include "user_interface.h"
#include "scene.h"
#include "debug.h"
#include "event.h"

using std::cerr;
using std::endl;
using std::max;

namespace grail {

void MainLoop::run() {
	Game &controller = Game::getInstance();
	
	uint32_t frameStart, frameEnd, frameDuration = 0;
	uint32_t statDuration = 0, statCycles = 0;
	
	while(!exit_) {
		frameStart = SDL_GetTicks();
		frameEnd = frameStart + static_cast<uint32_t>(1000.0 / targetFPS);
		
		// Do something
		controller.eachFrame(frameDuration);
		controller.renderEverything(frameDuration);
		
		// Handle events until frame end
		SDL_Event sdlEvent;
		while(SDL_PollEvent(&sdlEvent) != 0) {
			handleEvent(sdlEvent, frameDuration);
			freeUserEventData(sdlEvent);
		}
		SDL_Delay(max(0, static_cast<int32_t>(frameEnd - static_cast<int32_t>(SDL_GetTicks()))));
		
		frameDuration = SDL_GetTicks() - frameStart;
		
		statDuration += frameDuration;
		statCycles++;
		if(statDuration >= 5000) {
			cerr << "FPS (avg over 5s): "
				<< 1000.0 * static_cast<double>(statCycles) / static_cast<double>(statDuration) << endl;
			statDuration = 0;
			statCycles = 0;
		}
		
	} // while
}

void MainLoop::exit() { exit_ = true; }

void MainLoop::handleEvent(SDL_Event& event, uint32_t frameDuration) {
	Game &controller = Game::getInstance();
	
	if(event.type == SDL_QUIT) {
		exit();
	}
	
	else if(event.type == SDL_USEREVENT) {
	}
	
	if(controller.getUserInterface()->handleEvent(event, frameDuration) == EVENT_STATE_HANDLED) {
		return;
	}
	
	if(controller.getCurrentScene()->handleEvent(event, frameDuration) == EVENT_STATE_HANDLED) {
		return;
	}
	
}

} // namespace grail

