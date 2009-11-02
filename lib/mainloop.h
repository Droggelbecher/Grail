
#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <SDL/SDL.h>
#include "game.h"

class MainLoop {
  bool exit_;
  double targetFPS;

  public:
    MainLoop() : exit_(false), targetFPS(50.0) { };
    virtual ~MainLoop() { };

    virtual void run();
    virtual void exit();

    virtual void handleEvent(SDL_Event& event, uint32_t frameDuration);
};

#endif // MAINLOOP_H

