
#include "surface.h"
#include "utils.h"
#include "debug.h"
#include <SDL_rotozoom.h>

extern uint32_t g_scale;
namespace grail {

SDL_Surface* Surface::createSDLSurface(uint16_t w, uint16_t h, uint32_t flags) {
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
	
	SDL_Surface *s = SDL_CreateRGBSurface(flags, w, h, 32, rmask, gmask, bmask, amask);
	if(!s) {
		throw SDLException("Creation of surface failed");
	}
	return s;
}

void Surface::loadFromFile(const std::string& filename) {
	#ifdef WITH_OPENGL
		sdlSurface = rotozoomSurface(IMG_Load_RW(getRW(filename, MODE_READ), true), 0, (double)g_scale/1920, 0);
		if(!sdlSurface) {
			throw SDLException(std::string("Could not load surface '") + filename + "'");
		}
		SDL_SetColorKey(sdlSurface, SDL_RLEACCEL, sdlSurface->format->colorkey);
		buildGLTexture(sdlSurface);
	#else
		SDL_Surface* image = rotozoomSurface(IMG_Load_RW(getRW(filename, MODE_READ), true), 0, (double)g_scale/1920, 0);
		if(!image) {
			throw SDLException(std::string("Could not load surface '") + filename + "'");
		}
		SDL_SetColorKey(image, SDL_RLEACCEL, image->format->colorkey);
		sdlSurface = SDL_DisplayFormatAlpha(image);
		SDL_FreeSurface(image); image = 0;
	#endif

}


#if WITH_OPENGL
void Surface::buildGLTexture(SDL_Surface* surface) {
	uint16_t w = sdlSurface->w, h = sdlSurface->h;
	uint16_t w2 = nextPower2(w), h2 = nextPower2(h);
	textureWidth = (float)w / (float)w2;
	textureHeight = (float)h / (float)h2;

	SDL_Surface *padded;

	if(w != w2 || h != h2) {
		// Padding necessary
		padded = createSDLSurface(w2, h2, SDL_SWSURFACE | SDL_SRCALPHA);
		if(!padded) {
			throw SDLException(std::string("Could not allocate helper surface for OpenGL (low ram?)"));
		}
		SDL_SetColorKey(padded, SDL_RLEACCEL, padded->format->colorkey);
		SDL_SetAlpha(surface, 0, 255); // Make a "dumb" copy (ignore alpha during blit)
		SDL_BlitSurface(surface, 0, padded, 0);
	}
	else {
		padded = surface;
	}

	GLenum textureFormat;
	switch(padded->format->BytesPerPixel) {
		case 4: // with alpha channel
			if(padded->format->Rmask == 0x000000ff) { textureFormat = GL_RGBA; }
			else { textureFormat = GL_BGRA; }
			break;
		case 3: // without alpha channel
			if(padded->format->Rmask == 0x000000ff) { textureFormat = GL_RGB; }
			else { textureFormat = GL_BGR; }
			break;
		default:
			assert(false);
			break;
	}

	glGenTextures(1, &glTexture);
	glBindTexture(GL_TEXTURE_2D, glTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, padded->w, padded->h,
			0, textureFormat, GL_UNSIGNED_BYTE, padded->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	if(padded != surface) {
		SDL_FreeSurface(padded); padded = 0;
	}
}
#endif

Surface::Surface(const std::string &path) : sdlSurface(0) {
	loadFromFile(path);
}

Surface::Surface(PhysicalSize size, uint32_t flags) {
	sdlSurface = createSDLSurface(size.getX(), size.getY(), flags);
	buildGLTexture(sdlSurface);
}

Surface::Surface(PhysicalSize size, SDL_Color color, uint32_t flags) {
	sdlSurface = createSDLSurface(size.getX(), size.getY(), flags);
	SDL_FillRect(sdlSurface, 0, SDL_MapRGB(sdlSurface->format, color.r, color.g, color.b));

	buildGLTexture(sdlSurface);
}

Surface::Surface(SDL_Surface* s) : sdlSurface(s) {
	buildGLTexture(sdlSurface);
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
		uint16_t w = sdlSurface->w, h = sdlSurface->h;
		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
		glBindTexture(GL_TEXTURE_2D, glTexture);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0); glVertex3i(to->x, to->y, 0);
		glTexCoord2f(textureWidth, 0.0); glVertex3i(to->x + w, to->y, 0);
		glTexCoord2f(textureWidth, textureHeight); glVertex3i(to->x + w, to->y + h, 0);
		glTexCoord2f(0.0, textureHeight); glVertex3i(to->x, to->y + h, 0);
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

/*SDL_Surface* Surface::getSDL() {
	return sdlSurface;
}*/

} // namespace grail

