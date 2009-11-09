
#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>

#include "surface.h"
#include "animation.h"

class Sprite : public Animation {
  protected:
    std::vector<uint32_t> frameDurations;
    uint32_t currentFrame, frames;
    uint32_t defaultDuration;
    uint32_t ticksSinceFrameStart;

  public:
    Sprite(std::string classname) : Animation(classname) {
      defaultDuration = 100;
      currentFrame = 0;
      frames = 0;
      ticksSinceFrameStart = 0;
    }
    virtual ~Sprite() { };

    uint32_t getFrameDuration(size_t n) {
      if(n < frameDurations.size()) {
        return frameDurations[n];
      }
      return defaultDuration;
    }

    void setDefaultFrameDuration(uint32_t duration) { defaultDuration = duration; }
    void setFrameDurations(const std::vector<uint32_t>& durations);

    void eachFrame(uint32_t ticks);
    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
    virtual void renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const = 0;
};

#include <iostream>

class StripeSprite : public Sprite {
  protected:
    Surface* surface;
    PhysicalSize::X frameWidth;
  public:
    StripeSprite(std::string path, size_t frames) : Sprite("StripeSprite") {
      surface = new Surface(path);
      frameWidth = surface->getSize().getX() / frames;
      std::cerr  << "frameWidth:" << frameWidth << endl;
      this->frames = frames;
    }
    ~StripeSprite() {
      delete surface;
    }

    void renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const;
    VirtualSize getSize() const {
      return conv<PhysicalSize, VirtualSize>(
          PhysicalSize(frameWidth, surface->getSize().getY())
          );
    }
};

/*
class ImageSprite : public Sprite {
  protected:
    std::vector<Surface*> frames;

    Surface* getCurrentSurface();

  public:
    ImageSprite(std::string path) : Sprite("ImageSprite");
    void renderCurrentFrameAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
    VirtualSize getSize() const {
      return conv<PhysicalSize, VirtualSize>(getCurrentSurface().getSize());
    }
};
*/
#endif // SPRITE_H

