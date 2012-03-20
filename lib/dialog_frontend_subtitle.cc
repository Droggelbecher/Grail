// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		// sdefault subs position
		int defaultX = 2000;
		int defaultY = 2800;
		subtitlePosition = VirtualPosition(defaultX, defaultY);


		//center the subs by default
		centered = true;

		// show speaker's name by default
		showSpeakersName = true;

		// default font to be used
		setFont("fonts/crkdwno1.ttf");
	}

	void DialogFrontendSubtitle::say(DialogLine::Ptr line) {

		// set the default font here if not set
		// ** for some reason doing this in the constructor causes a segfault
		// and I can't figure out why -- feedelli **
		if (!defaultFont) {
			defaultFont = boost::shared_ptr<Font>(new Font(defaultFontPath, 30, 1));
		}

		std::string subtitleText = line->getText();

		// prepend the speakers name and a colon if showSpeakersName is set
		if (showSpeakersName) {	
			subtitleText = line->getSpeaker()->getName() + ": " + subtitleText;
			line->setText(subtitleText);
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

		// count active subs
		int activeSubtitles = 0;

		// render the subtitles centered
		// render backwards so earlier subtitles stack upwards
		for (std::vector<boost::shared_ptr<Subtitle> >::reverse_iterator riter = subtitles.rbegin();
			riter != subtitles.rend(); ++riter) {

			if ((*riter)->isStarted()) {

				activeSubtitles++;

				VirtualPosition renderPosition = subtitlePosition;

				// center the text around the rener position if set to centered
				if (centered) {
					renderPosition = renderPosition - ((*riter)->getSize()/2);
				}

				if (activeSubtitles > 1) {
					renderPosition.setY(renderPosition.getY() - (200 * (activeSubtitles - 1)));
				}

				(*riter)->renderAt(target, ticks, (renderPosition));
			}
		}
	}

	void DialogFrontendSubtitle::setFont(std::string path) {
		defaultFontPath = path;
	}

	void DialogFrontendSubtitle::setCentered(bool c) {
		centered = c;
	}
} // namespace grail

