
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

    ~Image() {
      delete &surface;
    }

    VirtualSize getSize() const {
      return conv<PhysicalSize, VirtualSize>(surface.getSize());
    }


    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
      SDL_Rect t = conv<PhysicalPosition, SDL_Rect>(
          conv<VirtualPosition, PhysicalPosition>(p)
          );
      surface.blit(NULL, target, &t);
    }
};

#endif // IMAGE_H

