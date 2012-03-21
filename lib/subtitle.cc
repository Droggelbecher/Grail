// vim: set noexpandtab:

#include "subtitle.h"

namespace grail {

	Subtitle::Subtitle(boost::shared_ptr<DialogLine> d, boost::shared_ptr<Font> f) : dialogLine(d), font(f), started(false), complete(false) {
		boost::shared_ptr<Text> t(new Text(font));
		t->setText(dialogLine->getText());
		text = t;

		// set the subtitle length to the same length as the dialog line
		timeToLive = dialogLine->getLength();
	}

	void Subtitle::start() {
		started = true;
		timeStarted = SDL_GetTicks();
	}

	void Subtitle::eachFrame(uint32_t ticks) {
		text->eachFrame(ticks);

		// start this subtitle if the dialog line has started
		if (!isStarted() && dialogLine->isStarted()) {
			start();
		}

		if (isStarted()) {
			uint32_t timeNow = SDL_GetTicks();

			if (timeNow > (timeStarted + timeToLive)) {
				complete = true;
			}
		}
	}

	void Subtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {
		text->renderAt(target, ticks, p);
	}

} // namespace grail

