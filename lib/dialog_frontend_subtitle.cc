// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"

namespace grail {

	Subtitle::Subtitle(DialogLine::Ptr d, Font::Ptr f) : dialogLine(d), font(f), started(false), complete(false) {

		// create the text object to represent the dialog line's text
		Text::Ptr t(new Text(font));
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

		// start this subtitle if the dialog line has started (if
		// actor has started saying it
		if (!isStarted() && dialogLine->isStarted()) {
			start();
		}

		// check if subtitle should finish displaying
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


	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		// default subs position
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
			defaultFont = Font::Ptr(new Font(defaultFontPath, 30, 1));
		}

		// prepend the speakers name and a colon if showSpeakersName is set
		std::string subtitleText = line->getText();

		if (showSpeakersName) {	
			subtitleText = line->getSpeaker()->getName() + ": " + subtitleText;
			line->setText(subtitleText);
		}

		// queue a new subtitle with the line to be said and using the default font for now
		// ** to be implemented: allow characters to have their own fonts
		Subtitle::Ptr s(new Subtitle(line, defaultFont));
		subtitles.push_back(s);
	}

	void DialogFrontendSubtitle::eachFrame(uint32_t ticks)  {

		// remove completed subs
		for (std::vector<Subtitle::Ptr>::iterator iter = subtitles.begin();
			iter != subtitles.end(); ) {
			if ((*iter)->isComplete()) {
				iter = subtitles.erase(iter);
			} else {
				++iter;
			}
		}

		// update the subtitles
		for (std::vector<Subtitle::Ptr>::iterator iter = subtitles.begin();
			iter != subtitles.end(); iter++) {
			(*iter)->eachFrame(ticks);
		}
	}

	void DialogFrontendSubtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) {

		// count active subs (to keep track of stacking subtitles
		// vertically on-screen
		int activeSubtitles = 0;

		// render the subtitles centered
		// note:: render backwards so earlier subtitles stack upwards
		for (std::vector<Subtitle::Ptr>::reverse_iterator riter = subtitles.rbegin();
			riter != subtitles.rend(); ++riter) {

			if ((*riter)->isStarted()) {

				activeSubtitles++;

				VirtualPosition renderPosition = subtitlePosition;

				// center the text around the render position if set to centered
				if (centered) {
					renderPosition = renderPosition - ((*riter)->getSize()/2);
				}

				// if more than one sub playing stack them up vertically
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

