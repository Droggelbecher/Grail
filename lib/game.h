
#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL/SDL.h>
#include "classes.h"
#include "registry.h"

enum EventState { EVENT_STATE_HANDLED, EVENT_STATE_UNHANDLED };

struct Event {
  enum Type {
    EVT_BLUBB = SDL_NUMEVENTS + 100,
  };
  int type;
  const SDL_Event& sdl;

  Event(SDL_Event& sdl) : sdl(sdl) {
    type = sdl.type;
  }
};

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

    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);


    void quit();
};

#endif // GAME_H

