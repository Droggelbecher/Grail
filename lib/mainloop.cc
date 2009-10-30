
#include <iostream>
#include <SDL/SDL.h>
#include "mainloop.h"
#include "game.h"
#include "utils.h"

using std::cerr;
using std::endl;

void MainLoop::run() {
  Game &controller = Game::getInstance();

  uint32_t frameStart, frameEnd, frameDuration = 0;
  uint32_t statDuration = 0, statCycles = 0;

  while(!exit_) {
    frameStart = SDL_GetTicks();
    frameEnd = frameStart + (uint32_t)(1000.0 / targetFPS);

    // Do something
    controller.renderEverything(frameDuration);

    // Handle events until frame end
    SDL_Event sdlEvent;
    while(SDL_PollEvent(&sdlEvent) != 0) {
      Event event(sdlEvent);
      handleEvent(event, frameDuration);
    }
    SDL_Delay(max(0, (int32_t)frameEnd - (int32_t)SDL_GetTicks()));

    frameDuration = SDL_GetTicks() - frameStart;

    statDuration += frameDuration;
    statCycles++;
    if(statDuration >= 5000) {
      cerr << "FPS (avg over 5s): "
        << 1000.0 * (double)statCycles / (double)statDuration << endl;
      statDuration = 0;
      statCycles = 0;
    }

  } // while
}

void MainLoop::exit() { exit_ = true; }

void MainLoop::handleEvent(Event& event, uint32_t frameDuration) {
  if(event.type == SDL_QUIT) {
    exit();
  }
}

