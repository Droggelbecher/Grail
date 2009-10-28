
#ifndef SCENE_H
#define SCENE_H

#include <cassert>
#include "vector2d.h"
#include "animation.h"
#include "registry.h"

#include <iostream>
using std::cerr;
using std::endl;

class Scene : public Registrable {
    const Animation* background;

  public:
    Scene() : Registrable("Scene"), background(NULL) { }
    ~Scene() {
      delete background;
    }

    void setBackground(const Animation& background) {
      this->background = &background;
    }

    void renderAt(SDL_Surface* target, uint32_t ticks, InternalPosition p) const {
      assert(target != NULL);
      if(background != NULL) {
        background->renderAt(target, ticks, p);
      }
    } // renderAt
};

#endif // SCENE_H

