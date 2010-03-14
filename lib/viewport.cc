// vim: set noexpandtab:

#include <SDL_video.h>

#include "utils.h"
#include "viewport.h"
#include "scene.h"
#include "game.h"

namespace grail {

Viewport::Viewport() : screen(0),
	virtualSize(VirtualSize(4000, 3000)), cameraMode(CAMERA_CENTER),
	cameraLimit(LIMIT_SCENE), cameraPosition(VirtualPosition(0, 0)) {
}

Viewport::Viewport(uint32_t w, uint32_t h, bool fullscreen) : screen(0) {
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
		Scene::Ptr scene = Game::getInstance().getCurrentScene();
		if(scene) {
			if(bottomRight.getX() >= scene->getSize().getX()) {
				cameraPosition.setX(scene->getSize().getX() - virtualSize.getX());
			}
			if(bottomRight.getY() >= scene->getSize().getY()) {
				cameraPosition.setY(scene->getSize().getY() - virtualSize.getY());
			}
		}
	} // if LIMIT_SCENE
} // setCameraPosition()

void Viewport::renderScene(Scene::Ptr scene, uint32_t ticks) {
	if(cameraTarget) {
		setCameraPosition(
				cameraTarget->getPosition() - (virtualSize / 2.0)
		);
	}
	scene->renderAt(screen, ticks, -cameraPosition);
}

void Viewport::renderUserInterface(UserInterface::Ptr userInterface, uint32_t ticks) {
	userInterface->renderAt(screen, ticks, VirtualPosition(0, 0));
}

void Viewport::finishRendering() {
	SDL_Flip(screen);
}

} // namespace grail

