
#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <SDL.h>

namespace grail {
  extern SDL_Color white;
  extern SDL_Color black;

  SDL_Color rgb(uint32_t v);
  SDL_Color rgb(uint8_t r, uint8_t g, uint8_t b);
}

#endif // SDLUTILS_H

