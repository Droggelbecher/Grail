
#ifndef SCENE_ANIMATION_H
#define SCENE_ANIMATION_H

#include "animation.h"

class SceneAnimation {
  protected:
    Animation *animation;
    VirtualPositon position;

  public:
    SceneAnimation(Animation animation) : animation(animation) { }

};

#endif // SCENE_ANIMATION_H

