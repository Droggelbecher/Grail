
#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL/SDL.h>
#include "classes.h"
#include "registry.h"
#include "event.h"

class Game : public Object {

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

