// vim: set noexpandtab:

#include <iostream>
#include <SDL.h>
#include "game.h"
#include "utils.h"
#include "viewport.h"
#include "resource_manager.h"
#include "user_interface.h"
#include "debug.h"

using std::cout;
using std::cerr;
using std::clog;
using std::endl;
using std::string;

namespace grail {

Game* Game::_instance = 0;

const std::string Game::className = "Game";

Game::Game() : viewport(0), resourceManager(0), loop(true) {
	SDL_Init(SDL_INIT_EVERYTHING);
}

Game::~Game() {
	delete viewport;
	delete resourceManager;
	SDL_Quit();
}

Game& Game::getInstance() {
	if(!_instance) {
		_instance = new Game();
	}
	return *_instance;
}

void Game::runChapter(size_t chapter) {
	initChapter(chapter);
	loop.run();
}

Viewport& Game::getViewport() {
	if(!viewport) { viewport = new Viewport(); }
	return *viewport;
}

void Game::registerScene(Scene::Ptr scene, string name) {
	scenes[name] = scene;
}

Scene::Ptr Game::getScene(string name) {
	return scenes[name];
}

void Game::clearScenes() {
	scenes.clear();
}

Scene::Ptr Game::getCurrentScene() const throw(ValueNotSet) {
	if(!currentScene) {
		throw ValueNotSet("Game doesn't have a current scene.");
	}
	return currentScene;
}

void Game::goToScene(Scene::Ptr scene) {
	currentScene = scene;
}

ResourceManager& Game::getResourceManager() {
	if(!resourceManager) { resourceManager = new ResourceManager(); }
	return *resourceManager;
}

void Game::setUserInterface(UserInterface::Ptr ui) {
	userInterface = ui;
}

UserInterface::Ptr Game::getUserInterface() {
	return userInterface;
}

void Game::setMainCharacter(Actor::Ptr actor) {
	mainCharacter = actor;
}

Actor::Ptr Game::getMainCharacter() const {
	return mainCharacter;
}

void Game::eachFrame(uint32_t ticks) {
	if(userInterface) {
		userInterface->eachFrame(ticks);
	}
	if(currentScene) {
		currentScene->eachFrame(ticks);
	}
}

void Game::renderEverything(uint32_t ticks) {
	if(viewport) {
		viewport->startRendering();
		if(currentScene) {
			viewport->renderScene(currentScene, ticks);
		}
		if(userInterface) {
			viewport->renderUserInterface(userInterface, ticks);
		}
		viewport->finishRendering();
	}
}

void Game::handleEvent(const SDL_Event &event, uint32_t ticks) {
}

void Game::quit() {
	loop.exit();
}

} // namespace grail

