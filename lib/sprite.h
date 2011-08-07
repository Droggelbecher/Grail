// vim: set noexpandtab:

#ifndef SPRITE_H
#define SPRITE_H

#include <vector>
#include <string>

#include "surface.h"
#include "animation.h"

namespace grail {

/**
 * Common (abstract) base class for all sprite types.
 */
class Sprite : public Animation {
	protected:
		std::vector<uint32_t> frameDurations;
		uint32_t currentFrame, frames;
		uint32_t defaultDuration;
		uint32_t ticksSinceFrameStart;
		
	public:
		Sprite(uint32_t frames = 0, uint32_t defaultDuration = 100) :
			frames(frames), defaultDuration(defaultDuration) {
			currentFrame = 0;
			ticksSinceFrameStart = 0;
		}
		
		virtual ~Sprite() { };
		
		uint32_t getFrameDuration(size_t n) {
			if(n < frameDurations.size()) {
				return frameDurations[n];
			}
			return defaultDuration;
		}
		
		void setDefaultFrameDuration(uint32_t duration) { defaultDuration = duration; }
		void setFrameDurations(const std::vector<uint32_t>& durations);
		
		void eachFrame(uint32_t ticks);
		void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
		virtual void renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const = 0;
};

/**
 * Sprite based on a single image file which contains the sprites frames
 * in a horizontal alignment (first frame is the leftmost)
 */
class StripeSprite : public Sprite {
	protected:
		Surface* surface;
		PhysicalSize::X frameWidth;
	public:
		StripeSprite(std::string path, size_t frames);
		StripeSprite(std::string path, size_t frames, uint32_t frameDuration);
		
		~StripeSprite() {
			delete surface;
		}
		
		void renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const;
		VirtualSize getSize() const {
			return conv<PhysicalSize, VirtualSize>(
				PhysicalSize(frameWidth, surface->getSize().getY())
			);
		}
};

/**
 * Sprite based on image files in a directory.
 * Frame order is determined by lexicographical order of file names.
 */
class ImageSprite : public Sprite {
	protected:
		std::vector<Surface::Ptr> surfaces;
		
		Surface::Ptr getCurrentSurface() const;
		
	public:
		ImageSprite(std::string dir, uint32_t defaultDuration = 100);
		void renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const;
		VirtualSize getSize() const;
};

} // namespace grail

#endif // SPRITE_H

