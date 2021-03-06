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
#include "dialog_frontend.h"
#include "actor.h"
#include "mainloop.h"
#include "vector2d.h"

namespace grail {

class Game {
	protected:
		static Game* _instance;
		
		Viewport* viewport;
		Scene::Ptr currentScene;
		ResourceManager* resourceManager;
		UserInterface::Ptr userInterface;
		DialogFrontend::Ptr dialogFrontend;
		Actor::Ptr mainCharacter;
		std::map<std::string, Scene::Ptr> scenes;
		MainLoop loop;
		bool userControl;
		
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
		DialogFrontend::Ptr getDialogFrontend();
		MainLoop& getMainLoop() { return loop; }
		
		Actor::Ptr getMainCharacter() const;
		void setMainCharacter(Actor::Ptr actor);
		
		virtual void eachFrame(uint32_t ticks);
		void renderEverything(uint32_t ticks);
		EventState handleEvent(SDL_Event &event, uint32_t ticks);
		
		void enableUserControl(bool enable);
		
		void quit();
		
		virtual void event(std::string type, Actor::Ptr actor) {};
		virtual void event(std::string type, Actor actor) {};
		virtual void event(std::string type, VirtualPosition position) {};
};

} // namespace grail

#endif // GAME_H

