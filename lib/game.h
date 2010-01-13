
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>

#include <SDL/SDL.h>
#include "classes.h"
#include "event.h"
#include "reference_counting.h"

namespace grail {

class Game : public ReferenceCounted {
  protected:

    static Game* _instance;

    double targetFPS;

    Viewport* viewport;
    Scene* currentScene;
    ResourceManager* resourceManager;
    UserInterface* userInterface;

    Game();

  public:
    static const std::string className;

    ~Game();

    static Game& getInstance();

    virtual void initChapter(size_t) { };
    void runChapter(size_t chapter);

    Viewport& getViewport();
    Scene& getCurrentScene() const;
    void goToScene(Scene& scene);
    ResourceManager& getResourceManager();
    void setUserInterface(UserInterface& ui);
    UserInterface& getUserInterface();

    void eachFrame(uint32_t ticks);
    void renderEverything(uint32_t ticks);
    void handleEvent(const SDL_Event &event, uint32_t ticks);

    void quit();
};

} // namespace grail

#endif // GAME_H

