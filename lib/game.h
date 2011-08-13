// vim: set noexpandtab:

#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <map>

#include <boost/shared_ptr.hpp>
#include <SDL.h>

#include "classes.h"
#include "event.h"
#include "exceptions.h"
#include "scene.h"
#include "user_interface.h"
#include "actor.h"
#include "mainloop.h"

namespace grail {

class Game {
	protected:
		static Game* _instance;
		
		Viewport* viewport;
		Scene::Ptr currentScene;
		ResourceManager* resourceManager;
		UserInterface::Ptr userInterface;
		Actor::Ptr mainCharacter;
		std::map<std::string, Scene::Ptr> scenes;
		MainLoop loop;
		
		Game();
		
	public:
		typedef boost::shared_ptr<Game> Ptr;
		
		virtual ~Game();
		
		static Game& getInstance();
		
		virtual void initChapter(size_t) { };
		void runChapter(size_t chapter);
		
		Viewport& getViewport();
		void registerScene(Scene::Ptr scene, std::string name);
		Scene::Ptr getScene(std::string name);
		Scene::Ptr getCurrentScene() const throw(ValueNotSet);
		void clearScenes();
		void goToScene(Scene::Ptr scene);
		ResourceManager& getResourceManager();
		void setUserInterface(UserInterface::Ptr ui);
		UserInterface::Ptr getUserInterface();
		MainLoop& getMainLoop() { return loop; }
		
		Actor::Ptr getMainCharacter() const;
		void setMainCharacter(Actor::Ptr actor);
		
		virtual void eachFrame(uint32_t ticks);
		void renderEverything(uint32_t ticks);
		void handleEvent(const SDL_Event &event, uint32_t ticks);
		
		void quit();
};

} // namespace grail

#endif // GAME_H

