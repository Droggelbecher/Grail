
#ifndef SURFACE_H
#define SURFACE_H

#include <cassert>
#include <string>
#include <iostream>
using std::cerr;
using std::endl;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "shortcuts.h"
#include "utils.h"

namespace grail {

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
    typedef boost::shared_ptr<Surface> Ptr;
    typedef boost::shared_ptr<const Surface> ConstPtr;

    Surface(std::string &path) : sdlSurface(0) {
      loadFromFile(path);
      assert(sdlSurface);
    }

    ~Surface() {
      if(sdlSurface) {
        SDL_FreeSurface(sdlSurface);
        sdlSurface = 0;
      }
    }

    PhysicalSize getSize() const {
      return PhysicalSize(sdlSurface->w, sdlSurface->h);
    }

    void blit(SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const {
      SDL_BlitSurface(sdlSurface, from, target, to);
    }
};

} // namespace grail

#endif // SURFACE_H

