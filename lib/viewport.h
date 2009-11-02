
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <SDL/SDL.h>

#include "scene.h"
#include "registry.h"

class Viewport : public Registrable {
    SDL_Surface* screen;
    VirtualSize virtualSize;

  public:

    Viewport();
    Viewport(uint32_t w, uint32_t h, bool fullscreen = false);
    void setup(uint32_t w, uint32_t h, bool fullscreen = false);

    VirtualSize getVirtualSize();
    VirtualSize::X getVirtualWidth();
    VirtualSize::Y getVirtualHeight();

    PhysicalSize::X getPhysicalWidth();
    PhysicalSize::Y getPhysicalHeight();

    void renderScene(const Scene& scene, uint32_t ticks);
};


#endif // VIEWPORT_H

