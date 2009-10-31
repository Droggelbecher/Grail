
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

    virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
    void setAlignment(double x, double y);

    //virtual VirtualPosition::X getWidth() const = 0;
    //virtual VirtualPosition::Y getHeight() const = 0;
    virtual VirtualSize getSize() const = 0;

  protected:
    VirtualPosition getUpperLeftCorner(VirtualPosition p) const;

};

#endif // ANIMATION_H

