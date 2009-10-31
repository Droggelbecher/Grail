
#ifndef ANIMATION_H
#define ANIMATION_H

#include <SDL/SDL.h>
#include <string>

#include "vector2d.h"
#include "registry.h"

class Animation : public Registrable {
    double alignmentX, alignmentY;

  public:
    Animation(std::string classname) : Registrable(classname), alignmentX(0.5), alignmentY(1.0) { }
    virtual ~Animation() { }

    virtual uint32_t getWidth() { return 0; }
    virtual uint32_t getHeight() { return 0; }

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
    void setAlignment(double x, double y);

  protected:
    VirtualPosition getUpperLeftCorner(VirtualPosition p);

};

#endif // ANIMATION_H

