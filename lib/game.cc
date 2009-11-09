
#include <iostream>
#include <SDL/SDL.h>
#include "game.h"
#include "utils.h"
#include "mainloop.h"
#include "viewport.h"
#include "resource_manager.h"
#include "user_interface.h"

using std::cout;
using std::cerr;
using std::clog;
using std::endl;

Game* Game::_instance = 0;

Game::Game() : Registrable("Game"), targetFPS(50.0), viewport(0), currentScene(0), resourceManager(0) {
  SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {
  delete viewport;
  delete resourceManager;
  delete userInterface;
  SDL_Quit();
}

Game& Game::getInstance() {
  if(!_instance) {
    _instance = new Game();
  }
  return *_instance;
}

void Game::runChapter(size_t chapter) {
  if(initChapter) {
    initChapter(chapter);
  }
  //run();
  MainLoop loop;
  loop.run();
}

Viewport& Game::getViewport() {
  if(!viewport) { viewport = new Viewport(); }
  return *viewport;
}

Scene& Game::getCurrentScene() const {
  return *currentScene;
}

void Game::goToScene(Scene& scene) {
  currentScene = &scene;
}

ResourceManager& Game::getResourceManager() {
  if(!resourceManager) { resourceManager = new ResourceManager(); }
  return *resourceManager;
}

Registry& Game::getRegistry() {
  return registry;
}

void Game::setUserInterface(UserInterface& ui) { userInterface = &ui; }
UserInterface& Game::getUserInterface() {
  assert(userInterface);
  return *userInterface;
}

void Game::eachFrame(uint32_t ticks) {
  if(currentScene) {
    currentScene->eachFrame(ticks);
  }
}

void Game::renderEverything(uint32_t ticks) {
  if(viewport && currentScene) {
    viewport->renderScene(*currentScene, ticks);
  }
}

void Game::handleEvent(const SDL_Event &event, uint32_t ticks) {
}



