// vim: set noexpandtab:

#include <SDL_video.h>

#include "utils.h"
#include "viewport.h"
#include "scene.h"
#include "game.h"

#ifdef WITH_OPENGL
	#include <GL/gl.h>
#endif
uint32_t g_scale;
namespace grail {

Viewport::Viewport() : screen(0),
	virtualSize(VirtualSize(4000, 3000)), cameraMode(CAMERA_CENTER),
	cameraLimit(LIMIT_SCENE), cameraPosition(VirtualPosition(0, 0)) {
}

Viewport::Viewport(uint32_t w, uint32_t h, bool fullscreen) : screen(0) {
	setup(w, h, fullscreen);
}

void Viewport::setup(uint32_t w, uint32_t h, bool fullscreen) {
	const SDL_VideoInfo *video = SDL_GetVideoInfo();
	int flags = SDL_HWPALETTE | (fullscreen ? SDL_FULLSCREEN : 0) | SDL_SWSURFACE;
	if(video) {
		if(video->hw_available) { flags |= SDL_HWSURFACE; }
		else { flags |= SDL_SWSURFACE; }
		if(video->blit_hw) { flags |= SDL_HWACCEL; }
	}

	#ifdef WITH_OPENGL
		flags |= SDL_OPENGL | SDL_GL_DOUBLEBUFFER;
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		screen = SDL_SetVideoMode(w, h, 32, flags | (fullscreen ? SDL_FULLSCREEN : 0));
	#endif
	screen = SDL_SetVideoMode(w, h, 32, flags);
	assert(screen != NULL);

	#ifdef WITH_OPENGL
		glEnable(GL_TEXTURE_2D);
		glShadeModel(GL_SMOOTH);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glViewport(0, 0, w, h);
		glClear(GL_COLOR_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		// Additive blending, nice for debugging
		//glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, w, h, 0, -1, 1);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	#endif
	g_scale = w;
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

void Viewport::startRendering() {
	#if WITH_OPENGL
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	#endif
}

void Viewport::finishRendering() {
	#ifdef WITH_OPENGL
		SDL_GL_SwapBuffers();
	#else
		SDL_Flip(screen);
	#endif
}

} // namespace grail

