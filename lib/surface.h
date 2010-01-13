
#ifndef SURFACE_H
#define SURFACE_H

#include <cassert>
#include <string>
#include <iostream>
using std::cerr;
using std::endl;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>

#include "vector2d.h"
#include "shortcuts.h"
#include "utils.h"

class Surface {
    SDL_Surface* sdlSurface;

    void loadFromFile(std::string filename) {
      sdlSurface = IMG_Load_RW(getRW(filename, MODE_READ), true);
      if(!sdlSurface) {
        throw Exception(std::string("Could not load surface '") + filename + "'");
      }
    }


    Surface() { }
    Surface(const Surface& s) { }
    const Surface& operator=(const Surface& s) { return *this; }

  public:

    Surface(std::string &path) : sdlSurface(NULL) {
      loadFromFile(path);
      assert(sdlSurface != NULL);
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

    void blit(SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const {
      SDL_BlitSurface(sdlSurface, from, target, to);
    }
};

#endif // SURFACE_H

