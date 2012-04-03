// vim: set noexpandtab:

#include "dialog_frontend_subtitle.h"
#include "game.h"

namespace grail {

	DialogFrontendSubtitle::DialogFrontendSubtitle() {

		// default subs position
		setPosition(VirtualPosition(2000,2800));

		useActorsFont = false;

		//center the subs by default
		centered = true;

		// show speaker's name by default
		showSpeakersName = true;
	}

	void DialogFrontendSubtitle::addLine(DialogLine::Ptr line) {
	
		// prepend the speakers name and a colon if showSpeakersName is set
		std::string subtitleText = line->getText();

		if (showSpeakersName && (line->getText() != "")) {	
			subtitleText = line->getSpeaker()->getName() + ": " + subtitleText;
			line->setText(subtitleText);
		}


		// queue a new subtitle with the line to be said
		// use actors font if they have one and the option is set
		Font::Ptr font;
		if (useActorsFont && line->getSpeaker()->getFont()) {
			font = line->getSpeaker()->getFont();
		} else {
			font = defaultFont;
		}
		DialogText::Ptr s(new DialogText(line, font));
		lines.push_back(s);
	}

	void DialogFrontendSubtitle::renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const {

		// count active subs (to keep track of stacking subtitles
		// vertically on-screen
		int activeSubtitles = 0;

		// render the subtitles centered
		// note:: render backwards so earlier subtitles stack upwards
		for (std::vector<DialogText::Ptr>::const_reverse_iterator riter = lines.rbegin();
			riter != lines.rend(); ++riter) {

			if ((*riter)->getDialogLine()->isStarted()) {

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

	bool DialogFrontendSubtitle::getUseActorsFont() {
		return useActorsFont;
	}

	void DialogFrontendSubtitle::setUseActorsFont(bool use) {
		useActorsFont = use;
	}
} // namespace grail

