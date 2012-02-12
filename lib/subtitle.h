// vim: set noexpandtab:

#ifndef SUBTITLE_H
#define SUBTITLE_H

#include "text.h"
#include "dialog_line.h"
#include "boost/shared_ptr.hpp"

namespace grail {

	// Displays actors' lines in subtitles on the screen
	class Subtitle {

		public:
			Subtitle(boost::shared_ptr<DialogLine>);

			void eachFrame(uint32_t ticks);
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;

			void start();
			bool isStarted() { return started; }
			bool isComplete() { return complete; }

		protected:
			boost::shared_ptr<Text> text;
			boost::shared_ptr<DialogLine> dialogLine;

			// timer stuff (note should be put in timer wrapper
			uint32_t timeToLive;
			uint32_t timeStarted;
			bool started;
			bool complete;
	};

} // namespace grail

#endif // SUBTITLE_H

