
#include "animation.h"

#include <cassert>

namespace grail {

const std::string Animation::className = "Animation";

const std::string DirectionAnimation::className = "DirectionAnimation";

DirectionAnimation::DirectionAnimation(uint16_t directions) :
  Animation("DirectionAnimation"), currentDirection(0), directions(directions) {
  assert(directions > 0);
  animations = new Animation::Ptr[directions];
}

DirectionAnimation::~DirectionAnimation() {
  delete[] animations;
}

void DirectionAnimation::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition position) const {
  assert(directions > currentDirection);
  animations[currentDirection]->renderAt(target, ticks, position);
}

VirtualSize DirectionAnimation::getSize() const {
  assert(directions > currentDirection);
  return animations[currentDirection]->getSize();
}

bool DirectionAnimation::hasPoint(VirtualPosition p) const {
  assert(directions > currentDirection);
  return animations[currentDirection]->hasPoint(p);
}

void DirectionAnimation::eachFrame(uint32_t ticks) {
  assert(directions > currentDirection);
  return animations[currentDirection]->eachFrame(ticks);
}

void DirectionAnimation::setAnimation(uint16_t direction, Animation::Ptr animation) {
  assert(direction < directions);
  animations[direction] = animation;
}

} // namespace grail


