
#ifndef SDLUTILS_H
#define SDLUTILS_H

#include <SDL/SDL.h>

namespace grail {
  extern SDL_Color white;
  extern SDL_Color black;

  SDL_Color rgb(uint32_t v);
}

#endif // SDLUTILS_H

