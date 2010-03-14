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
#include <boost/shared_ptr.hpp>

#include "vector2d.h"
#include "shortcuts.h"
#include "utils.h"
#include "sdl_exception.h"

namespace grail {

class Surface {
		SDL_Surface* sdlSurface;
		
		void loadFromFile(const std::string& filename) {
			sdlSurface = IMG_Load_RW(getRW(filename, MODE_READ), true);
			if(!sdlSurface) {
				throw new SDLException(std::string("Could not load surface '") + filename + "'");
			}
			SDL_SetColorKey(sdlSurface, SDL_RLEACCEL, sdlSurface->format->colorkey);
			SDL_Surface* newSurface = SDL_DisplayFormatAlpha(sdlSurface);
			SDL_FreeSurface(sdlSurface);
			sdlSurface = newSurface;
		}
		
		Surface() { }
		Surface(const Surface& s) { }
		const Surface& operator=(const Surface& s) { return *this; }
		
	public:
		typedef boost::shared_ptr<Surface> Ptr;
		
		/**
		 * Create new surface from image resource
		 */
		Surface(const std::string &path) : sdlSurface(0) {
			loadFromFile(path);
			assert(sdlSurface);
		}
		
		/**
		 * Create new surface with given size and flags.
		 */
		Surface(PhysicalSize size, uint32_t flags = SDL_HWSURFACE) {
			uint32_t rmask, gmask, bmask, amask;
			
			// Stolen from: http://www.libsdl.org/cgi/docwiki.cgi/SDL_CreateRGBSurface
			/* SDL interprets each pixel as a 32-bit number, so our masks must depend
				 on the endianness (byte order) of the machine */
			#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
			#else
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
			#endif
			
			sdlSurface = SDL_CreateRGBSurface(flags, size.getX(), size.getY(), 32, rmask, gmask, bmask, amask);
			if(!sdlSurface) {
				throw new SDLException("Creation of surface failed");
			}
		}
		
		/**
		 * Adopt given sdl surface.
		 */
		Surface(SDL_Surface* s) : sdlSurface(s) {
		}
		
		~Surface() {
			if(sdlSurface) {
				SDL_FreeSurface(sdlSurface);
				sdlSurface = 0;
			}
		}
		
		PhysicalSize getSize() const {
			if(!sdlSurface) {
				return PhysicalSize();
			}
			return PhysicalSize(sdlSurface->w, sdlSurface->h);
		}
		
		void blit(SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const {
			if(sdlSurface) {
				SDL_BlitSurface(sdlSurface, from, target, to);
			}
		}
		
		void blit(PhysicalPosition from, SDL_Surface* target, PhysicalPosition to) const {
			if(sdlSurface) {
				SDL_Rect f = conv<PhysicalPosition, SDL_Rect>(from);
				f.w = sdlSurface->w;
				f.h = sdlSurface->h;
				SDL_Rect t = conv<PhysicalPosition, SDL_Rect>(to);
				SDL_BlitSurface(sdlSurface, &f, target, &t);
			}
		}
		
		uint8_t getAlpha(PhysicalPosition p) const {
			if(!sdlSurface || p.getX() < 0 || p.getX() >= sdlSurface->w ||
					p.getY() < 0 || p.getY() >= sdlSurface->h) {
				return 0;
			}
			
			if(sdlSurface->format->BitsPerPixel != 32) {
				throw new Exception("Only 32-Bit-Surfaces are supported atm.");
			}
			
			SDL_LockSurface(sdlSurface);
			uint32_t pixel = ((const uint32_t*)sdlSurface->pixels)[p.getY() * sdlSurface->w + p.getX()];
			SDL_UnlockSurface(sdlSurface);
			uint8_t r, g, b, a;
			SDL_GetRGBA(pixel, sdlSurface->format, &r, &g, &b, &a);
			return a;
		}
		
		SDL_Surface* getSDL() { return sdlSurface; }
};

} // namespace grail

#endif // SURFACE_H

