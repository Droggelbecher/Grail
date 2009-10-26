
#ifndef GAME_H
#define GAME_H

#include <iostream>

#include <SDL/SDL.h>
#include "viewport.h"
#include "scene.h"

class Game {

    double targetFPS;

    Viewport* viewport;
    Scene* currentScene;

    void run();

  public:
    Game();
    ~Game();

    void (*initChapter)(size_t);
    void runChapter(size_t chapter);

    Viewport& getViewport() {
      if(viewport == NULL) {
        viewport = new Viewport();
      }
      return *viewport;
    }

    Scene& getCurrentScene() const { return *currentScene; };
    void goToScene(Scene& scene) { currentScene = &scene; };

    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);
};

#endif // GAME_H

