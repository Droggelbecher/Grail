// vim: set noexpandtab:

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "actor.h"
#include "wait_task.h"

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

			// timer to keep track of how long line being spoken
			WaitTask::Ptr timer;

		public:
			typedef boost::shared_ptr<DialogLine> Ptr;

			DialogLine(Actor::Ptr, std::string, uint32_t);

			Actor::Ptr getSpeaker() { return speaker; };
			void setText(std::string newText) { text = newText; };
			std::string getText() { return text; };

			void start();
			bool isStarted();
			bool isComplete();

			void eachFrame();
	};

} // namespace grail

#endif // DIALOGLINE_H

