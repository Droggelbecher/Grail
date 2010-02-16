
#include <algorithm>
#include <string>
#include <vector>

#include "sprite.h"

#include "resource_manager.h"
#include "debug.h"
#include "game.h"

using std::string;
using std::vector;
using std::copy;
using std::sort;

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

//
//
//

StripeSprite::StripeSprite(std::string path, size_t frames) :
  Sprite(frames), surface(new Surface(path)),
  frameWidth(surface->getSize().getX() / frames) {
}

StripeSprite::StripeSprite(std::string path, size_t frames, uint32_t frameDuration) :
  Sprite(frames, frameDuration), surface(new Surface(path)),
  frameWidth(surface->getSize().getX() / frames) {
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

//
//
//

Surface::Ptr ImageSprite::getCurrentSurface() const {
  if(currentFrame < surfaces.size()) {
    return surfaces[currentFrame];
  }
  return Surface::Ptr();
}

ImageSprite::ImageSprite(string dir, uint32_t defaultDuration) :
  Sprite(0, defaultDuration) {
    
  ResourceManager::DirectoryIterator iter = Game::getInstance().getResourceManager().beginListing(dir);
  vector<string> paths;

  for(;iter!=Game::getInstance().getResourceManager().endListing(); iter++) {
    paths.push_back(*iter);
  }

  sort(paths.begin(), paths.end());

  for(vector<string>::iterator i=paths.begin(); i!=paths.end(); ++i) {
    surfaces.push_back(Surface::Ptr(new Surface(dir + "/" + *i)));
  }
  frames = surfaces.size();
}

void ImageSprite::renderCurrentFrameAt(SDL_Surface* target, VirtualPosition p) const {
  Surface::Ptr s = getCurrentSurface();
  if(s) {
    s->blit(PhysicalPosition(), target, conv<VirtualPosition, PhysicalPosition>(p));
  }
}

VirtualSize ImageSprite::getSize() const {
  Surface::Ptr s = getCurrentSurface();
  if(s) {
    return conv<PhysicalSize, VirtualSize>(getCurrentSurface()->getSize());
  }
  return VirtualSize();
}

} // namespace grail


