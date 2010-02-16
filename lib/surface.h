
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
#include "sdl_exception.h"

namespace grail {

class Surface {
    SDL_Surface* sdlSurface;

    void loadFromFile(const std::string& filename) {
      sdlSurface = IMG_Load_RW(getRW(filename, MODE_READ), true);
      if(!sdlSurface) {
        throw new SDLException(std::string("Could not load surface '") + filename + "'");
      }
    }


    Surface() { }
    Surface(const Surface& s) { }
    const Surface& operator=(const Surface& s) { return *this; }

  public:
    typedef boost::shared_ptr<Surface> Ptr;
    typedef boost::shared_ptr<const Surface> ConstPtr;

    /**
     * Create new surface from image resource
     */
    Surface(const std::string &path) : sdlSurface(0) {
      loadFromFile(path);
      assert(sdlSurface);
    }

    /**
     * Create new surface with given size and flags.
     */
    Surface(PhysicalSize size, uint32_t flags = SDL_SWSURFACE) {
      uint32_t rmask, gmask, bmask, amask;

      // Stolen from: http://www.libsdl.org/cgi/docwiki.cgi/SDL_CreateRGBSurface
      /* SDL interprets each pixel as a 32-bit number, so our masks must depend
         on the endianness (byte order) of the machine */
      #if SDL_BYTEORDER == SDL_BIG_ENDIAN
        rmask = 0xff000000;
        gmask = 0x00ff0000;
        bmask = 0x0000ff00;
        amask = 0x000000ff;
      #else
        rmask = 0x000000ff;
        gmask = 0x0000ff00;
        bmask = 0x00ff0000;
        amask = 0xff000000;
      #endif

      sdlSurface = SDL_CreateRGBSurface(flags, size.getX(), size.getY(), 32, rmask, gmask, bmask, amask);
      if(!sdlSurface) {
        throw new SDLException("Creation of surface failed");
      }
    }

    /**
     * Adopt given sdl surface.
     */
    Surface(SDL_Surface* s) : sdlSurface(s) {
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

    void blit(PhysicalPosition from, SDL_Surface* target, PhysicalPosition to) const {
      SDL_Rect f = conv<PhysicalPosition, SDL_Rect>(from);
      f.w = sdlSurface->w;
      f.h = sdlSurface->h;
      SDL_Rect t = conv<PhysicalPosition, SDL_Rect>(to);
      SDL_BlitSurface(sdlSurface, &f, target, &t);
    }

    SDL_Surface* getSDL() { return sdlSurface; }
};

} // namespace grail

#endif // SURFACE_H

