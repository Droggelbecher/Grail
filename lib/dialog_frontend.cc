// vim: set noexpandtab:

#include "dialog_frontend.h"
#include "vector2d.h"

namespace grail {

	void DialogFrontend::say(DialogLine::Ptr line, Actor::Ptr speaker) {
		
		// if the speaker does not already have a text object associated
		// with them create it now
		if (!lines[speaker]) {

			// temporary solution to create text object
			boost::shared_ptr<Font> font(new Font("fonts/tommy_holloway.ttf", 40, 1));
			boost::shared_ptr<Text> text(new Text(font));

			text->setText(line->getText());

			lines[speaker] = text;
		}
	}
}

