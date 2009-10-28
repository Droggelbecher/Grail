
#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL/SDL.h>
#include "viewport.h"
#include "scene.h"
#include "resource_manager.h"
#include "registry.h"

class Game;

class Game : public Registrable {

    double targetFPS;
    Viewport* viewport;
    Scene* currentScene;
    ResourceManager* resourceManager;
    static Game* _instance;
    Registry registry;

    Game();
    void run();

  public:
    ~Game();

    static Game& instance() {
      if(!_instance) {
        _instance = new Game();
      }
      return *_instance;
    }

    void (*initChapter)(size_t);
    void runChapter(size_t chapter);

    Viewport& getViewport() {
      if(!viewport) { viewport = new Viewport(); }
      return *viewport;
    }

    Scene& getCurrentScene() const { return *currentScene; };
    void goToScene(Scene& scene) { currentScene = &scene; };

    ResourceManager& getResourceManager() {
      if(!resourceManager) { resourceManager = new ResourceManager(); }
      return *resourceManager;
    }

    Registry& getRegistry() { return registry; }

    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);
};

#endif // GAME_H

