// vim: set noexpandtab:

#include <iostream>
#include <SDL.h>
#include "game.h"
#include "utils.h"
#include "viewport.h"
#include "resource_manager.h"
#include "user_interface.h"
#include "debug.h"
#include "dialog_frontend_subtitle.h"

using std::cout;
using std::cerr;
using std::clog;
using std::endl;
using std::string;

namespace grail {

Game* Game::_instance = 0;

Game::Game() : viewport(0), resourceManager(0), loop(true), userControl(true) {
	SDL_Init(SDL_INIT_EVERYTHING);

	// temporarily use default dialog frontend
	dialogFrontendSubtitle = new DialogFrontendSubtitle();
	dialogFrontend = boost::shared_ptr<DialogFrontend>(dialogFrontendSubtitle);
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
	currentScene->onEnter();
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

DialogFrontend::Ptr Game::getDialogFrontend() {
	return dialogFrontend;
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
	if(dialogFrontend) {
		dialogFrontend->eachFrame(ticks);
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

EventState Game::handleEvent(SDL_Event &event, uint32_t ticks) {
	
	if(event.type == SDL_QUIT) {
		loop.exit();
	}

	if(userControl) {
		if(getUserInterface()->handleEvent(event, ticks) == EVENT_STATE_HANDLED) {
			return EVENT_STATE_HANDLED;
		}
		
		if(getCurrentScene()->handleEvent(event, ticks) == EVENT_STATE_HANDLED) {
			return EVENT_STATE_HANDLED;
		}
	}
	return EVENT_STATE_UNHANDLED;
}

void Game::enableUserControl(bool enable) {
	userControl = enable;
}

void Game::quit() {
	loop.exit();
}

} // namespace grail

