// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_SUBTITLE_H
#define DIALOG_FRONTEND_SUBTITLE_H

#include "dialog_frontend.h"
#include "font.h"
#include "text.h"
#include "boost/shared_ptr.hpp"

namespace grail {

	/**
	* Represents a textual subtitle which appears on-screen
	*/
	class Subtitle {
			DialogLine::Ptr dialogLine; // dialog line the subtitle represents
			Font::Ptr font; // font used for subtitles
			Text::Ptr text; // text object representing the subtitle

		public:
			typedef boost::shared_ptr<Subtitle> Ptr;

			Subtitle(DialogLine::Ptr, Font::Ptr);

			/**
			* Start display of the subtitle
			*/
			void start();

			VirtualSize getSize() const { return text->getSize(); }
			DialogLine::Ptr getDialogLine() { return dialogLine; }

			void eachFrame(uint32_t ticks);
			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
	};

	/**
	* Pluggable dialog frontend which displays
	* actors' lines in subtitles on the screen
	*/
	class DialogFrontendSubtitle : public DialogFrontend {

			/**
			* the subtitles which are queued to be displayed
			*/
			std::vector<Subtitle::Ptr> subtitles;

			/**
			* the default font used to render subtitles
			* (if an actor does not specifiy own font)
			*/
			Font::Ptr defaultFont;

			/**
			* display options
			*/
			bool centered;	//is the text centered
			bool showSpeakersName; // prepend text with actors name and colon
			bool useActorsFont; // use the actors own font for the sub

		public:
			DialogFrontendSubtitle();

			/*
 			* create a subtitle for a given line of dialog
 			*/
			void createSubtitle(DialogLine::Ptr);

			Font::Ptr getFont();
			void setFont(Font::Ptr);

			bool getCentered();
			void setCentered(bool);

			bool getShowSpeakersName();
			void setShowSpeakersName(bool);

			bool getUseActorsFont();
			void setUseActorsFont(bool);

			void eachFrame(uint32_t ticks);
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
	};

} // namespace grail

#endif // DIALOG_FRONTEND_SUBTITLE_H

