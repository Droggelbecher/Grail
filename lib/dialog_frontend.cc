// vim: set noexpandtab:

#include "dialog_frontend.h"
#include "game.h"

namespace grail {

	void DialogFrontend::eachFrame(uint32_t ticks) {
		updateDialogLines();
	}

	std::list<DialogLine::Ptr> DialogFrontend::updateDialogLines() {

		std::list<DialogLine::Ptr> newLines; // remember which ones are new

		// get all the actors currently in the scene
		std::list<Actor::Ptr> actors = Game::getInstance().getCurrentScene()->getActors();

		// collect each actor's lines and add them to the list if not already there
		Actor::Ptr actor;
		for (std::list<Actor::Ptr>::iterator iter = actors.begin();
			iter != actors.end(); ++iter) {
			actor = (*iter);	
			if (actor->isSpeaking()) {
				DialogLine::Ptr line = actor->getDialogLine();

				// check if the line is already in the list
				if (!(std::find(lines.begin(), lines.end(), line) != lines.end()) && (line->getText() != "")) {
					lines.push_back(line);	
					newLines.push_back(line);
				}
			}
		}

		return newLines;
	}
}

