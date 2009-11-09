
#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL/SDL.h>
#include "classes.h"
#include "registry.h"
#include "event.h"

/**
 * WARNING: Game is a "Registrable" so it can be easily wrapped in lua but you
 * should NEVER put a game object into the registry.
 */
class Game : public Registrable {

    static Game* _instance;

    double targetFPS;

    Viewport* viewport;
    Scene* currentScene;
    ResourceManager* resourceManager;
    Registry registry;
    UserInterface* userInterface;

    Game();

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
    void setUserInterface(UserInterface& ui);
    UserInterface& getUserInterface();

    void eachFrame(uint32_t ticks);
    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);


    void quit();
};

#endif // GAME_H

