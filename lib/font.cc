// vim: set noexpandtab:

#include "font.h"
#include "resource_manager.h"
#include "sdl_exception.h"
#include "debug.h"
#include "game.h"

namespace grail {
	Font::Font(std::string path, int size, int outline) : path(path), size(size) {
		if(!TTF_WasInit()) {
			TTF_Init();
		}
		
		SDL_RWops *rw = Game::getInstance().getResourceManager().getRW(path, MODE_READ);
		font = TTF_OpenFontRW(rw, 1, virtualSizeToPhysicalSize(size));
		if(!font) {
			throw SDLException(std::string("Couldnt load font '") + path + "'");
		}
	}
	
	Font::Font(const Font& other) {
		if(!TTF_WasInit()) {
			TTF_Init();
		}
		
		path = other.path;
		size = other.size;
		
		SDL_RWops *rw = Game::getInstance().getResourceManager().getRW(path, MODE_READ);
		font = TTF_OpenFontRW(rw, 1, virtualSizeToPhysicalSize(other.size));
		if(!font) {
			throw SDLException(std::string("Couldnt load font '") + path + "'");
		}
	}
	
	Font::~Font() {
		if(font) {
			TTF_CloseFont(font);
		}
	}

	int Font::virtualSizeToPhysicalSize(int vs) {
		return vs * Game::getInstance().getViewport().getPhysicalHeight() / 800.0;
	}
	
	void Font::setOutline(int width) {
		//TTF_SetFontOutline(font, width);
	}
	
	int Font::getOutline() const {
		return 0;
		//return TTF_GetFontOutline(font);
	}
	
} // namespace

