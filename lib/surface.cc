
#include "surface.h"

namespace grail {

void Surface::loadFromFile(const std::string& filename) {
	sdlSurface = IMG_Load_RW(getRW(filename, MODE_READ), true);
	if(!sdlSurface) {
		throw SDLException(std::string("Could not load surface '") + filename + "'");
	}
	SDL_SetColorKey(sdlSurface, SDL_RLEACCEL, sdlSurface->format->colorkey);
	SDL_Surface* newSurface = SDL_DisplayFormatAlpha(sdlSurface);
	SDL_FreeSurface(sdlSurface);
	sdlSurface = newSurface;

	buildGLTexture();
}

void Surface::buildGLTexture() {
	#if WITH_OPENGL
		// TODO: Check if sdlSurface->w and sdlSurface->h are powers of 2
		// See http://gpwiki.org/index.php/SDL:Tutorials:Using_SDL_with_OpenGL)
		// TODO: Check if we really need to keep the sdlSurface around
		GLenum textureFormat;
		switch(sdlSurface->format->BytesPerPixel) {
			case 4: // with alpha channel
				if(sdlSurface->format->Rmask == 0x000000ff) { textureFormat = GL_RGBA; }
				else { textureFormat = GL_BGRA; }
				break;
			case 3: // without alpha channel
				if(sdlSurface->format->Rmask == 0x000000ff) { textureFormat = GL_RGB; }
				else { textureFormat = GL_BGR; }
				break;
			default:
				assert(false);
				break;
		}

		glGenTextures(1, &glTexture);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sdlSurface->w, sdlSurface->h,
				0, textureFormat, GL_UNSIGNED_BYTE, sdlSurface->pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//SDL_FreeSurface(sdlSurface);
		//sdlSurface = 0;
	#endif
}

Surface::Surface(const std::string &path) : sdlSurface(0) {
	loadFromFile(path);
	assert(sdlSurface);
	buildGLTexture();
}

Surface::Surface(PhysicalSize size, uint32_t flags) {
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
		throw SDLException("Creation of surface failed");
	}
	buildGLTexture();
}

Surface::Surface(SDL_Surface* s) : sdlSurface(s) {
	buildGLTexture();
}

Surface::~Surface() {
	#ifdef WITH_OPENGL
		glDeleteTextures(1, &glTexture);
	#endif

	if(sdlSurface) {
		SDL_FreeSurface(sdlSurface);
		sdlSurface = 0;
	}
}

PhysicalSize Surface::getSize() const {
	if(!sdlSurface) {
		return PhysicalSize();
	}
	return PhysicalSize(sdlSurface->w, sdlSurface->h);
}

void Surface::blit(SDL_Rect* from, SDL_Surface* target, SDL_Rect* to) const {
	#ifdef WITH_OPENGL
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glBegin(GL_QUADS);
		glTexCoord2i(0, 0); glVertex3i(to->x, to->y, 0);
		glTexCoord2i(1, 0); glVertex3i(to->x + sdlSurface->w, to->y, 0);
		glTexCoord2i(1, 1); glVertex3i(to->x + sdlSurface->w, to->y + sdlSurface->h, 0);
		glTexCoord2i(0, 1); glVertex3i(to->x, to->y + sdlSurface->h, 0);
		glEnd();
	#else
		if(sdlSurface) {
			SDL_BlitSurface(sdlSurface, from, target, to);
		}
	#endif // WITH_OPENGL
}

void Surface::blit(PhysicalPosition from, SDL_Surface* target, PhysicalPosition to) const {
	if(sdlSurface) {
		SDL_Rect f = conv<PhysicalPosition, SDL_Rect>(from);
		f.w = sdlSurface->w;
		f.h = sdlSurface->h;
		SDL_Rect t = conv<PhysicalPosition, SDL_Rect>(to);
		blit(&f, target, &t);
	}
}

uint8_t Surface::getAlpha(PhysicalPosition p) const {
	if(!sdlSurface || p.getX() < 0 || p.getX() >= sdlSurface->w ||
			p.getY() < 0 || p.getY() >= sdlSurface->h) {
		return 0;
	}
	
	if(sdlSurface->format->BitsPerPixel != 32) {
		throw Exception("Only 32-Bit-Surfaces are supported atm.");
	}
	
	SDL_LockSurface(sdlSurface);
	uint32_t pixel = ((const uint32_t*)sdlSurface->pixels)[p.getY() * sdlSurface->w + p.getX()];
	SDL_UnlockSurface(sdlSurface);
	uint8_t r, g, b, a;
	SDL_GetRGBA(pixel, sdlSurface->format, &r, &g, &b, &a);
	return a;
}

SDL_Surface* Surface::getSDL() {
	return sdlSurface;
}

} // namespace grail

