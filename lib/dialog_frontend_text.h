// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_TEXT_H
#define DIALOG_FRONTEND_TEXT_H

#include "dialog_frontend.h"
#include "font.h"
#include "text.h"

namespace grail {

	/**
	* Represents an actor's dialog line as text
	*/
	class DialogText {
			DialogLine::Ptr dialogLine; // dialog line to represent
			Font::Ptr font;
			Text::Ptr text; // text object for display

		public:
			typedef boost::shared_ptr<DialogText> Ptr;

			DialogText(DialogLine::Ptr, Font::Ptr);

			/**
			* Start display of the text
			*/
			void start();

			VirtualSize getSize() const { return text->getSize(); }
			DialogLine::Ptr getDialogLine() { return dialogLine; }

			void eachFrame(uint32_t ticks);

			void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const;
	};


	/**
	* Abstract base class for dialog frontends
	* that use text to display dialog - subtitle 
	* and speech bubble frontends
	*/
	class DialogFrontendText : public DialogFrontend {

		protected:
			/**
			* the dialog lines which are queued to be displayed
			*/
			std::vector<DialogText::Ptr> lines;

			/**
			* the default font used to render text
			*/
			Font::Ptr defaultFont;

		public:
			DialogFrontendText();

			/*
 			* create a DialogText for a given line of dialog
 			*/
			virtual void addLine(DialogLine::Ptr);

			Font::Ptr getFont();
			void setFont(Font::Ptr);

			virtual void eachFrame(uint32_t ticks);
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;
	};

} // namespace grail

#endif //DIALOG_FRONTEND_SPEECH_BUBBLES_H

