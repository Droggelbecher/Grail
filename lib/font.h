// vim: set noexpandtab:

#ifndef FONT_H
#define FONT_H

#include <SDL.h>
#include <SDL_ttf.h>
#include "game.h"
#include "viewport.h"

namespace grail {
	class Font {
			std::string path;
			int size;
			TTF_Font* font;
			
			int virtualSizeToPhysicalSize(int vs) {
				return vs * Game::getInstance().getViewport().getPhysicalHeight() / 800.0;
			}
			
		public:
			typedef boost::shared_ptr<Font> Ptr;
			
			Font(std::string path, int size, int outline);
			Font(const Font& other);
			virtual ~Font();
			
			void setOutline(int width);
			int getOutline() const;
			TTF_Font* getSDL() const { return font; }
	};
}

#endif

