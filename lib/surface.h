
#ifndef SURFACE_H
#define SURFACE_H

#include <cassert>
#include <string>
#include <iostream>
using std::cerr;
using std::endl;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "shortcuts.h"

class Surface {
    SDL_Surface* sdlSurface;
    SDL_Surface* backup;

    void loadFromFile(std::string filename) {
      sdlSurface = IMG_Load_RW(getRW(filename, MODE_READ), true);
      backup = sdlSurface;
      assert(sdlSurface != NULL);
      assert(sdlSurface == backup);
    }


    Surface() { }
    Surface(const Surface& s) { }
    const Surface& operator=(const Surface& s) { return *this; }

  public:

    Surface(std::string &path) : sdlSurface(NULL) {
      loadFromFile(path);
      assert(sdlSurface != NULL);
      assert(sdlSurface == backup);
    }

    ~Surface() {
      if(sdlSurface != NULL) {
        SDL_FreeSurface(sdlSurface);
        sdlSurface = NULL;
      }
    }

    PhysicalSize getSize() const {
      return PhysicalSize(sdlSurface->w, sdlSurface->h);
    }

    void blit(const SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const {
      assert(sdlSurface == backup);
      SDL_BlitSurface(sdlSurface, NULL, target, NULL);
    }
};

#endif // SURFACE_H

