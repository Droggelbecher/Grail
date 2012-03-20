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

		protected:
			std::string defaultFontPath;
			Font::Ptr defaultFont;
			VirtualPosition subtitlePosition;
			bool centered;	//is the text centered around subtitleposition
			bool showSpeakersName;

			std::vector<boost::shared_ptr<Subtitle> > subtitles;

		public:
			DialogFrontendSubtitle();
			void say(DialogLine::Ptr, Actor::Ptr);

			void eachFrame(uint32_t ticks);
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p);

			void setFont(std::string);
			void setCentered(bool); //set whether text should be centered or not
	};

} // namespace grail

#endif // DIALOG_FRONTEND_SUBTITLE_H

