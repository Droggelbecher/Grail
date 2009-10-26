
#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <iostream>
#include <cassert>
using std::cerr;
using std::endl;

#include <SDL/SDL.h>

#include "surface.h"
#include "vector2d.h"

class Image {
  protected:
    const Surface& surface;

  public:
    /*
    Image(const Surface& surface) : surface(surface) {
      cerr << "WTF" << endl;
    }
    */

    Image(std::string path) : surface(*(new Surface(path))) {
      //cerr << "Created image: " << &surface << endl;
    }

    void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const {
      SDL_Rect t = conv<InternalPosition, SDL_Rect>(p);
      surface.blit(NULL, target, &t);
    }
};

#endif // IMAGE_H

