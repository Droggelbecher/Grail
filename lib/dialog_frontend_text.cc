// vim: set noexpandtab:

#include "dialog_frontend_text.h"

namespace grail {

	// DialogText

	DialogText::DialogText(DialogLine::Ptr d, Font::Ptr f) : dialogLine(d), font(f) {

		// create the text object to represent the dialog line's text
		Text::Ptr t(new Text(font));
		t->setText(dialogLine->getText());
		text = t;
	}

	void DialogText::eachFrame(uint32_t ticks) {
		text->eachFrame(ticks);
	}

	void DialogText::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
		text->renderAt(target, ticks, p);
	}


	// DialogFrontendText

	DialogFrontendText::DialogFrontendText() {
		// default font to use
		defaultFont = Font::Ptr(new Font("fonts/crkdwno1.ttf", 30, 1));
	}	

	void DialogFrontendText::addLine(DialogLine::Ptr line) {
		DialogText::Ptr s(new DialogText(line, defaultFont));
		lines.push_back(s);
	}

	void DialogFrontendText::eachFrame(uint32_t ticks) {
		
		// remove completed lines
		for (std::vector<DialogText::Ptr>::iterator iter = lines.begin();
			iter != lines.end(); ) {
			// if the dialog line is gone or complete
			if ((!(*iter)->getDialogLine()) || (*iter)->getDialogLine()->isComplete()) {
				iter = lines.erase(iter);
			} else {
				++iter;
			}
		}
		
		// create new dialogtexts for new lines of dialog as necessary
		std::list<DialogLine::Ptr> newLines = updateDialogLines();
		if (!newLines.empty()) {
			if (newLines.size() > 1) {
				for(std::list<DialogLine::Ptr>::iterator iter = newLines.begin();
				iter != newLines.end(); ++iter) {
					addLine((*iter));
				}
			} else {
				addLine((*newLines.begin()));
			}
		}

		// update the lines
		for (std::vector<DialogText::Ptr>::iterator iter = lines.begin();
			iter != lines.end(); iter++) {
			(*iter)->eachFrame(ticks);
		}
	}

} // namespace grail

