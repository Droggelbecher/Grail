
#ifndef SCENE_H
#define SCENE_H

#include <algorithm>
#include <string>

#include <boost/shared_ptr.hpp>

#include "classes.h"
#include "vector2d.h"
#include "animation.h"
#include "event.h"
#include "actor.h"

namespace grail {

class Scene {
    struct Parallax {
      Animation::Ptr animation;
      VirtualPosition offset;
      double scrollFactorX, scrollFactorY;
      Parallax(Animation::Ptr animation, VirtualPosition offset, double scrollFactorX, double scrollFactorY) :
        animation(animation), offset(offset), scrollFactorX(scrollFactorX), scrollFactorY(scrollFactorY) {
      }
    }; // Parallax

    Animation::Ptr background;
    std::list<Parallax*> backgrounds, foregrounds;
    std::list<Actor::Ptr> actors;
    bool _actorsMoved;


  public:
    typedef boost::shared_ptr<Scene> Ptr;
    typedef boost::shared_ptr<const Scene> ConstPtr;

    static const std::string className;

    Scene();
    Scene(Animation::Ptr);
    Scene(const std::string& backgroundPath);

    virtual ~Scene();

    VirtualSize getSize() const;

    void setBackground(Animation::Ptr background);

    void addBackground(Animation::Ptr background, VirtualPosition offset = VirtualPosition(), double scrollFactorX = 1.0, double scrollFactorY = 1.0);
    void addForeground(Animation::Ptr forgeground, VirtualPosition offset = VirtualPosition(), double scrollFactorX = 1.0, double scrollFactorY = 1.0);

    void addActor(Actor::Ptr entity) {
      actors.push_back(entity);
      std::inplace_merge(actors.begin(), actors.end(), actors.end(), Actor::CompareByY());
    }

    void actorsMoved() { _actorsMoved = true; }

    void eachFrame(uint32_t ticks);
    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;

    EventState handleEvent(SDL_Event& event, uint32_t ticks);
};

} // namespace grail

#endif // SCENE_H

