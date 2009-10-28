
#include <SDL/SDL_video.h>

#include "viewport.h"
#include "scene.h"

Viewport::Viewport() : Registrable("Viewport"), screen(NULL) {
}

Viewport::Viewport(uint32_t w, uint32_t h, bool fullscreen) : Registrable("Viewport"), screen(NULL) {
  setup(w, h, fullscreen);
}

void Viewport::setup(uint32_t w, uint32_t h, bool fullscreen) {
  screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | (fullscreen ? SDL_FULLSCREEN : 0));
  assert(screen != NULL);
}

void Viewport::renderScene(const Scene& scene, uint32_t ticks) {
  scene.renderAt(screen, ticks, InternalPosition(0, 0));
  SDL_Flip(screen);
}

