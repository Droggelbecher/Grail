// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		int defaultX = 1300;
		int defaultY = 2800;
		subtitlePosition = VirtualPosition(defaultX, defaultY);

		setFont("fonts/crkdwno1.ttf");
	}

	void DialogFrontendSubtitle::say(DialogLine::Ptr line, Actor::Ptr speaker) {

		// set the default font here if not set
		// ** for some reason doing this in the constructor causes a segfault
		// and I can't figure out why -- feedelli **
		if (!defaultFont) {
			defaultFont = boost::shared_ptr<Font>(new Font(defaultFontPath, 40, 1));
		}

		// queue a new subtitle with the line to be said and using the default font for now
		// ** to be implemented: allow characters to have their own fonts
		boost::shared_ptr<Subtitle> s(new Subtitle(line, defaultFont));
		subtitles.push_back(s);
	}

	void DialogFrontendSubtitle::eachFrame(uint32_t ticks)  {

		// remove completed subs
		for (std::vector<boost::shared_ptr<Subtitle> >::iterator iter = subtitles.begin();
			iter != subtitles.end(); ) {
			if ((*iter)->isComplete()) {
				iter = subtitles.erase(iter);
			} else {
				++iter;
			}
		}

		// update the subtitles
		for (std::vector<boost::shared_ptr<Subtitle> >::iterator iter = subtitles.begin();
			iter != subtitles.end(); iter++) {
			(*iter)->eachFrame(ticks);
		}
	}

	void DialogFrontendSubtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) {

		// render the subtitles
		for (std::vector<boost::shared_ptr<Subtitle> >::iterator iter = subtitles.begin();
			iter != subtitles.end(); iter++) {
			if ((*iter)->isStarted()) {
				(*iter)->renderAt(target, ticks, subtitlePosition);
			}
		}
	}

	void DialogFrontendSubtitle::setFont(std::string path) {
		defaultFontPath = path;
	}
} // namespace grail

