// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"
#include "game.h"

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
		setPosition(VirtualPosition(2000,2800));

		// default font to use
		defaultFont = Font::Ptr(new Font("fonts/crkdwno1.ttf", 30, 1));

		//center the subs by default
		centered = true;

		// show speaker's name by default
		showSpeakersName = true;
	}

	void DialogFrontendSubtitle::createSubtitle(DialogLine::Ptr line) {

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
			// if the subs dialog line is gone or complete
			if ((!(*iter)->getDialogLine()) || (*iter)->isComplete()) {
				iter = subtitles.erase(iter);
			} else {
				++iter;
			}
		}
		
		// create new subs for new lines of dialog as necessary
		std::list<DialogLine::Ptr> newLines = updateDialogLines();
		if (!newLines.empty()) {
			if (newLines.size() > 1) {
				for(std::list<DialogLine::Ptr>::iterator iter = newLines.begin();
				iter != newLines.end(); ++iter) {
					createSubtitle((*iter));
				}
			} else {
				createSubtitle((*newLines.begin()));
			}
		}

		// update the subtitles
		for (std::vector<Subtitle::Ptr>::iterator iter = subtitles.begin();
			iter != subtitles.end(); iter++) {
			(*iter)->eachFrame(ticks);
		}
	}

	void DialogFrontendSubtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {

		// count active subs (to keep track of stacking subtitles
		// vertically on-screen
		int activeSubtitles = 0;

		// render the subtitles centered
		// note:: render backwards so earlier subtitles stack upwards
		for (std::vector<Subtitle::Ptr>::const_reverse_iterator riter = subtitles.rbegin();
			riter != subtitles.rend(); ++riter) {

			if ((*riter)->isStarted()) {

				activeSubtitles++;

				VirtualPosition renderPosition = getPosition();

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

	Font::Ptr DialogFrontendSubtitle::getFont() {
		return defaultFont;
	}

	void DialogFrontendSubtitle::setFont(Font::Ptr f) {
		defaultFont = f;
	}

	bool DialogFrontendSubtitle::getCentered() {
		return centered;
	}

	void DialogFrontendSubtitle::setCentered(bool c) {
		centered = c;
	}

	bool DialogFrontendSubtitle::getShowSpeakersName() {
		return showSpeakersName;
	}

	void DialogFrontendSubtitle::setShowSpeakersName(bool show) {
		showSpeakersName = show;
	}
} // namespace grail

