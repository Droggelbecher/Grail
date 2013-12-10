// vim: set noexpandtab:

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <string>

#include <SDL.h>

#include "scene.h"
#include "user_interface.h"

namespace grail {

/**
 * Representation of the users screen.
 */
class Viewport {
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
		Actor::Ptr cameraTarget;
		
	public:
		///
		Viewport();
		///
		Viewport(uint32_t w, uint32_t h, bool fullscreen = false);
		
		/**
		 * Same as constructor with same signature for late initialization.
		 */
		void setup(uint32_t w, uint32_t h, bool fullscreen = false);
		
		VirtualSize getVirtualSize() const;
		
		PhysicalSize::X getPhysicalWidth() const;
		PhysicalSize::Y getPhysicalHeight() const;
		
		VirtualPosition getCameraPosition() { return cameraPosition; }
		void setCameraPosition(VirtualPosition position);
		
		void renderScene(Scene::Ptr scene, uint32_t ticks);
		void renderUserInterface(UserInterface::Ptr userInterface, uint32_t ticks);
		void startRendering();
		void finishRendering();
		
		void setFollowing(Actor::Ptr actor) {
			cameraTarget = actor;
		}
		
		void setNoFollowing() {
			cameraTarget.reset();
		}

		void setWindowTitle(const char* title, const char* icon);
};

} // namespace grail


#endif // VIEWPORT_H

