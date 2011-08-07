// vim: set noexpandtab:

#ifndef ACTION_H
#define ACTION_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "actor.h"

namespace grail {
	
	/**
	 * Action a user can choose for an actor.
	 * (E.g. "take", "talk to", etc..)
	 */
	class Action {
			std::string verb;
			size_t subjects;
			bool allowOnScene;
			
		public:
			typedef boost::shared_ptr<Action> Ptr;
			
			/**
			 * \param verb Verb of the action e.g. "take", "talk to", etc...
			 * \param subjects Number of subjects this verb refers to e.g.
			 *                 "talk to" would usually refer to a single subject
			 *                 whereas "combine" would refer to two subjects.
			 * \param allowScene if true, a position on the scene is allowed
			 *                   as one of the subjects. Useful for verbs like
			 *                   "walk to".
			 */
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

