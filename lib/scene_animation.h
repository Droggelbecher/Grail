
#ifndef SCENE_ANIMATION_H
#define SCENE_ANIMATION_H

#include "animation.h"

namespace grail {

class SceneAnimation {
  protected:
    Animation *animation;
    VirtualPositon position;

  public:
    SceneAnimation(Animation animation) : animation(animation) { }

};

} // namespace grail

#endif // SCENE_ANIMATION_H

