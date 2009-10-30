
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

    static Game& getInstance();

    void (*initChapter)(size_t);
    void runChapter(size_t chapter);

    Viewport& getViewport();
    Scene& getCurrentScene() const;
    void goToScene(Scene& scene);

    ResourceManager& getResourceManager();
    Registry& getRegistry();

    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);
};

#endif // GAME_H

