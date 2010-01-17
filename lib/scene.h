
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
    Animation::ConstPtr background;

    std::list<Actor*> actors;
    bool _actorsMoved;

  public:
    typedef boost::shared_ptr<Scene> Ptr;
    typedef boost::shared_ptr<const Scene> ConstPtr;

    static const std::string className;

    Scene();
    Scene(const Animation& background);
    Scene(const std::string& backgroundPath);

    virtual ~Scene();

    VirtualSize getSize() const;

    void setBackground(const Animation& background);

    void addActor(Actor& entity) {
      actors.push_back(&entity);
      std::inplace_merge(actors.begin(), actors.end(), actors.end(), Actor::CompareByY());
    }

    void actorsMoved() { _actorsMoved = true; }

    void eachFrame(uint32_t ticks);
    void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;

    EventState handleEvent(SDL_Event& event, uint32_t ticks);
};

} // namespace grail

#endif // SCENE_H

