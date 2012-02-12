// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		int defaultX = 1300;
		int defaultY = 2800;
		subtitlePosition = VirtualPosition(defaultX, defaultY);
	}

	void DialogFrontendSubtitle::say(DialogLine::Ptr line, Actor::Ptr speaker) {
		boost::shared_ptr<Subtitle> s(new Subtitle(line));
		subtitles.push_back(s);
	}

	void DialogFrontendSubtitle::eachFrame(uint32_t ticks)  {

		// remove completed subs
		for (std::vector<boost::shared_ptr<Subtitle> >::iterator iter = subtitles.begin();
			iter != subtitles.end(); iter++) {
			if ((*iter)->isComplete()) {
				subtitles.erase(iter);
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
} // namespace grail

