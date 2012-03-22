// vim: set noexpandtab:

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "actor.h"

namespace grail {

	class DialogLine {
			/**
			* reference to the actor who is speaking the line
			*/
			Actor::Ptr speaker;

			/**
			* the text being said
			*/
			std::string text;

			// timing-related
			uint32_t timeToLive;
			uint32_t timeStarted;
			bool started;
			bool complete;

		public:
			typedef boost::shared_ptr<DialogLine> Ptr;

			DialogLine(Actor::Ptr, std::string, uint32_t);

			Actor::Ptr getSpeaker() { return speaker; };
			void setText(std::string newText) { text = newText; };
			std::string getText() { return text; };

			void start();
			bool isStarted() { return started; }
			bool isComplete() { return complete; }
			uint32_t getLength() { return timeToLive; }

			void eachFrame();
	};

} // namespace grail

#endif // DIALOGLINE_H

