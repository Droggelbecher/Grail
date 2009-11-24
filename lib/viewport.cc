
#include <SDL/SDL_video.h>

#include "utils.h"
#include "viewport.h"
#include "scene.h"

Viewport::Viewport() : Object("Viewport", false), screen(NULL),
  virtualSize(VirtualSize(4000, 3000)), cameraMode(CAMERA_CENTER),
  cameraLimit(LIMIT_SCENE), cameraPosition(VirtualPosition(0, 0)),
  cameraTarget(0) {
}

Viewport::Viewport(uint32_t w, uint32_t h, bool fullscreen) : Object("Viewport", false), screen(NULL) {
  setup(w, h, fullscreen);
}

void Viewport::setup(uint32_t w, uint32_t h, bool fullscreen) {
  screen = SDL_SetVideoMode(w, h, 32, SDL_SWSURFACE | (fullscreen ? SDL_FULLSCREEN : 0));
  assert(screen != NULL);
}

VirtualSize Viewport::getVirtualSize() const { return virtualSize; }

PhysicalSize::X Viewport::getPhysicalWidth() const {
  if(!screen)
    throw Exception("getWidth() called before setup()");

  return screen->w;
}
PhysicalSize::Y Viewport::getPhysicalHeight() const {
  if(!screen)
    throw Exception("getWidth() called before setup()");
  return screen->h;
}

void Viewport::setCameraPosition(VirtualPosition position) {
  cameraPosition = position;

  if(cameraLimit == LIMIT_SCENE) {
    if(cameraPosition.getX() < 0) { cameraPosition.setX(0); }
    if(cameraPosition.getY() < 0) { cameraPosition.setY(0); }
    VirtualPosition bottomRight = cameraPosition + virtualSize;
    Scene& scene = Game::getInstance().getCurrentScene();
    if(&scene) {
      if(bottomRight.getX() >= scene.getSize().getX()) {
        cameraPosition.setX(scene.getSize().getX() - virtualSize.getX());
      }
      if(bottomRight.getY() >= scene.getSize().getY()) {
        cameraPosition.setY(scene.getSize().getY() - virtualSize.getY());
      }
    }
  } // if LIMIT_SCENE
} // setCameraPosition()

void Viewport::renderScene(const Scene& scene, uint32_t ticks) {
  if(cameraTarget) {
    setCameraPosition(
        cameraTarget->getPosition() - (virtualSize / 2.0)
    );
  }
  scene.renderAt(screen, ticks, -cameraPosition);
  SDL_Flip(screen);
}

