// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_SUBTITLES_H
#define DIALOG_FRONTEND_SUBTITLES_H

#include "dialog_frontend_text.h"
#include "font.h"
#include "text.h"
#include "boost/shared_ptr.hpp"

namespace grail {

	/**
	* Pluggable dialog frontend which displays
	* actors' lines in subtitles on the screen
	*/
	class DialogFrontendSubtitles : public DialogFrontendText {

			/**
			* display options
			*/
			bool centered;	//is the text centered
			bool showSpeakersName; // prepend text with actors name and colon
			bool useActorsFont; // use the actors own font for the sub

		public:
			DialogFrontendSubtitles();

			void addLine(DialogLine::Ptr);

			Font::Ptr getFont();
			void setFont(Font::Ptr);

			bool getCentered();
			void setCentered(bool);

			bool getShowSpeakersName();
			void setShowSpeakersName(bool);

			bool getUseActorsFont();
			void setUseActorsFont(bool);

			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
	};

} // namespace grail

#endif // DIALOG_FRONTEND_SUBTITLES_H

