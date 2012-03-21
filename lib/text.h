// vim: set noexpandtab:

#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <boost/shared_ptr.hpp>
#include "vector2d.h"
#include "animation.h"
#include "font.h"
#include "sdlutils.h"
#include "blit_cached.h"

namespace grail {

/**
 * Render a piece of text using SDL_ttf
 */
class Text : public Animation, public BlitCached {
		std::string text;
		Font::Ptr font;
		Font::Ptr outlineFont;
		SDL_Color color, outlineColor;
		int outline;
		
	public:
		typedef boost::shared_ptr<Text> Ptr;
		
		Text(Font::Ptr font, Font::Ptr outlineFont);
		Text(Font::Ptr font);
		virtual ~Text() { }
		
		SDL_Color getColor() const { return color; }
		void setColor(SDL_Color c) { color = c; setChanged(); }
		
		SDL_Color getOutlineColor() const { return outlineColor; }
		void setOutlineColor(SDL_Color c) { outlineColor = c; setChanged(); }
		
		std::string getText() const { return text; }
		void setText(std::string t) {
			if(text != t) {
				text = t;
				setChanged();
			}
		}
		
		void setOutlineFont(Font::Ptr f, int outline) {
			this->outline = outline;
			outlineFont = f;
			setChanged();
		}
		
		void setOutline(int outline);
		
		Surface* render(int i) const;
		
		VirtualSize getSize() const {
			if(surface[1]) {
				return conv<PhysicalSize, VirtualSize>(surface[1]->getSize());
			}
			else if(surface[0]) {
				return conv<PhysicalSize, VirtualSize>(surface[0]->getSize());
			}
			return VirtualSize();
		}
		
		void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
			BlitCached::renderAt(target, ticks, p);
		}
		
		void eachFrame(uint32_t ticks) {
			BlitCached::eachFrame(ticks);
		}
};

}

#endif // TEXT_H

