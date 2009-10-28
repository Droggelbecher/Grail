
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
#include "animation.h"

class Image : public Animation {
  protected:
    const Surface& surface;

  public:
    Image(std::string path) : Animation("Image"), surface(*(new Surface(path))) {
    }

    void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const {
      SDL_Rect t = conv<InternalPosition, SDL_Rect>(p);
      surface.blit(NULL, target, &t);
    }
};

#endif // IMAGE_H

