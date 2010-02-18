
#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "actor.h"

namespace grail {

  class Action {
      std::string verb;
      size_t subjects;
      bool allowOnScene;

    public:
      typedef boost::shared_ptr<Action> Ptr;

      Action(std::string verb, size_t subjects, bool allowScene = false) :
        verb(verb), subjects(subjects), allowOnScene(allowOnScene) {
      }

      size_t getSubjectCount() const { return subjects; }
      bool allowedOnScene() const { return allowOnScene; }

      std::string getWording(const std::list<Actor::Ptr>& actors) const {
        std::string r = verb, comma = "";
        for(std::list<Actor::Ptr>::const_iterator iter = actors.begin(); iter != actors.end(); ++iter) {
          r += comma + (*iter)->getName();
          comma = ", ";
        }
        return r;
      }


  };

}

#endif // ACTION_H

