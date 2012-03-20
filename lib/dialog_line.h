// vim: set noexpandtab:

#ifndef DIALOGLINE_H
#define DIALOGLINE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "SDL.h"
#include "actor.h"

namespace grail {

	class DialogLine {

		public:
			typedef boost::shared_ptr<DialogLine> Ptr;

			DialogLine(Actor::Ptr, std::string, uint32_t);

			void eachFrame();

			void setText(std::string newText) { text = newText; };
			std::string getText() { return text; };

			Actor::Ptr getSpeaker() { return speaker; };

			void start();
			bool isStarted();
			bool isComplete();
			uint32_t getLength() { return timeToLive; }

		protected:
			boost::shared_ptr<Actor> speaker;
			std::string text;

			uint32_t timeToLive;
			uint32_t timeStarted;

			bool started;	// whether the dialog line is being said
			bool complete;	// whether the dialog line is finished being said
	};

} // namespace grail

#endif // DIALOGLINE_H

