
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <string>

#include <SDL/SDL.h>

#include "scene.h"
#include "reference_counting.h"

class Viewport : public ReferenceCounted {
    SDL_Surface* screen;
    VirtualSize virtualSize;

    enum CameraMode {
      CAMERA_STATIC,
      CAMERA_CENTER
    };
    enum CameraLimit {
      LIMIT_SCENE
    };

    CameraMode cameraMode;
    CameraLimit cameraLimit;
    VirtualPosition cameraPosition;
    Actor* cameraTarget;

  public:
    static const std::string className;

    Viewport();
    Viewport(uint32_t w, uint32_t h, bool fullscreen = false);
    void setup(uint32_t w, uint32_t h, bool fullscreen = false);

    VirtualSize getVirtualSize() const;

    PhysicalSize::X getPhysicalWidth() const;
    PhysicalSize::Y getPhysicalHeight() const;

    VirtualPosition getCameraPosition() { return cameraPosition; }
    void setCameraPosition(VirtualPosition position);
    void renderScene(const Scene& scene, uint32_t ticks);

    void keepCentering(Actor& actor) {
      cameraTarget = &actor;
    }
};


#endif // VIEWPORT_H

