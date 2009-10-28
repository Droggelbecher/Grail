
#include <iostream>
#include <SDL/SDL.h>
#include "game.h"
#include "utils.h"

using std::cout;
using std::cerr;
using std::clog;
using std::endl;

Game* Game::_instance = 0;

void Game::run() {
  bool exit = false;
  uint32_t frameStart, frameEnd, frameDuration = 0;
  uint32_t statDuration = 0, statCycles = 0;

  while(!exit) {
    frameStart = SDL_GetTicks();
    frameEnd = frameStart + (uint32_t)(1000.0 / targetFPS);

    // Do something
    renderEverything(frameDuration);

    // Handle events until frame end
    SDL_Event event;
    while(SDL_PollEvent(&event) != 0) {
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
} // Game::run()

Game::Game() : Registrable("Game"), targetFPS(50.0), viewport(0), currentScene(0), resourceManager(0) {
  SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {
  delete viewport;
  SDL_Quit();
}

void Game::runChapter(size_t chapter) {
  if(initChapter) {
    initChapter(chapter);
  }
  run();
}

void Game::renderEverything(uint32_t ticks) {
  if(viewport && currentScene) {
    viewport->renderScene(*currentScene, ticks);
  }
}

void Game::handleEvent(const SDL_Event &event, uint32_t ticks) {
}

