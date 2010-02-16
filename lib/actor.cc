
#include "actor.h"

#include <algorithm>
using std::copy;
#include <cassert>

#include "game.h"
#include "scene.h"

namespace grail {

const std::string Actor::className = "Actor";

void Actor::setAlignment(double x, double y) {
  alignmentX = x;
  alignmentY = y;
}

VirtualPosition Actor::getUpperLeftCorner() const {
  if(animation) {
    return VirtualPosition(
        position.getX() - alignmentX * animation->getSize().getX(),
        position.getY() - alignmentY * animation->getSize().getY()
        );
  }
  else {
    return position;
  }
}

void Actor::walk(const Path& path) {
  walkPath.clear();
  copy(path.begin(), path.end(), walkPath.begin());
  assert(walkPath.size() == path.size());
}

void Actor::walkStraight(VirtualPosition p) {
  walkPath.clear();
  walkPath.push_back(p);
}

void Actor::eachFrame(uint32_t ticks) {
  if(animation) {
    animation->eachFrame(ticks);
  }

  if(walkPath.empty()) {
    setMode("default");
  }
  else {
    setMode("walk");
    VirtualPosition target = walkPath.front();
    VirtualPosition diff = target - position;

    if(animation && diff.length()) {
      animation->setDirection(diff);
    }

    if(diff.length() <= (speed * ticks / 1000.0)) {
      position = target;
    }
    else {
      double steplen = (speed * ticks / (1000.0 * diff.length()));
      VirtualPosition step = diff * steplen;

      position = position + step;
    }

    if(diff.length()) {
      Game::getInstance().getCurrentScene()->actorsMoved();
    }

    if(position == target) {
      walkPath.pop_front();
    }
  } // if walkPath not empty
}

std::ostream& operator<<(std::ostream& os, const Actor& actor) {
  os << actor.getName();
  return os;
}

} // namespace grail

