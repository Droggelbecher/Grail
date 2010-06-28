// vim: set noexpandtab:

#ifndef SURFACE_H
#define SURFACE_H

#include <cassert>
#include <string>
#include <iostream>
using std::cerr;
using std::endl;

#include <SDL.h>
#include <SDL_image.h>
#ifdef WITH_OPENGL
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif
#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "shortcuts.h"
#include "utils.h"
#include "sdl_exception.h"

namespace grail {

class Surface {
		SDL_Surface* sdlSurface;
	#ifdef WITH_OPENGL
		GLuint glTexture;

		// When using OpenGL we destroy the sdl surface as soon as possible
		// and thus need to save some information about it
		//uint16_t w, h;
		float textureWidth, textureHeight;
	#endif
		
		static SDL_Surface* createSDLSurface(uint16_t w, uint16_t h, uint32_t flags = SDL_HWSURFACE);
		void loadFromFile(const std::string& filename);

	#ifdef WITH_OPENGL
		void buildGLTexture(SDL_Surface* surface);
	#else
		inline void buildGLTexture(SDL_Surface* surface) { }
	#endif
		
		// Forbid copying and default construction
		Surface() { }
		Surface(const Surface& s) { }
		const Surface& operator=(const Surface& s) { return *this; }
		
	public:
		typedef boost::shared_ptr<Surface> Ptr;
		
		/**
		 * Create new surface from image resource
		 */
		Surface(const std::string &path);
		
		/**
		 * Create new surface with given size and flags.
		 */
		Surface(PhysicalSize size, uint32_t flags = SDL_HWSURFACE);
		Surface(PhysicalSize size, SDL_Color color, uint32_t flags = SDL_HWSURFACE);

		/**
		 * Adopt given sdl surface.
		 */
		Surface(SDL_Surface* s);
		
		~Surface();
		
		PhysicalSize getSize() const;
		void blit(SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const;
		void blit(PhysicalPosition from, SDL_Surface* target, PhysicalPosition to) const;
		uint8_t getAlpha(PhysicalPosition p) const;
		//SDL_Surface* getSDL();
};

} // namespace grail

#endif // SURFACE_H

