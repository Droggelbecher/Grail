
#include "sprite.h"

#include "resource_manager.h"

using std::string;
using std::vector;

namespace grail {

void Sprite::setFrameDurations(const vector<uint32_t>& durations) {
  frameDurations = durations;
}

void Sprite::eachFrame(uint32_t ticks) {
  ticksSinceFrameStart += ticks;
  if(ticksSinceFrameStart >= getFrameDuration(currentFrame)) {
    ticksSinceFrameStart -= getFrameDuration(currentFrame);
    currentFrame++;
    if(currentFrame >= frames) {
      currentFrame = 0;
    }
  }
} // eachFrame

void Sprite::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
  renderCurrentFrameAt(target, p);
}


void StripeSprite::renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const {

  SDL_Rect from;
  from.x = currentFrame * frameWidth;
  from.y = 0;
  from.h = surface->getSize().getY();
  from.w = frameWidth;

  SDL_Rect to = conv<VirtualPosition, SDL_Rect>(p);
  
  surface->blit(&from, target, &to);
}

} // namespace grail


