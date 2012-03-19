// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_SUBTITLE_H
#define DIALOG_FRONTEND_SUBTITLE_H

#include "dialog_frontend.h"
#include "subtitle.h"
#include "font.h"
#include "boost/shared_ptr.hpp"

namespace grail {

	// Displays actors' lines in subtitles on the screen
	class DialogFrontendSubtitle : public DialogFrontend {

		public:
			DialogFrontendSubtitle();
			void say(DialogLine::Ptr, Actor::Ptr);

			void eachFrame(uint32_t ticks);
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p);

			void setFont(std::string);

		protected:
			std::string defaultFontPath;
			Font::Ptr defaultFont;
			VirtualPosition subtitlePosition;
			std::vector<boost::shared_ptr<Subtitle> > subtitles;
	};

} // namespace grail

#endif // DIALOG_FRONTEND_SUBTITLE_H

