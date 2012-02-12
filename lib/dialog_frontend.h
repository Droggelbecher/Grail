// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_H
#define DIALOG_FRONTEND_H

#include <boost/shared_ptr.hpp>
#include <map>

#include "dialog_line.h"
#include "actor.h"
#include "text.h"

namespace grail {

	// Virtual base class for in-game dialog renderers.
	// Intended to be pluggable so a developer may customise how dialog
	// is outputted to the screen.
	// For example via subtitles or text floating above an actor's head.
	class DialogFrontend {

		public:
			typedef boost::shared_ptr<DialogFrontend> Ptr;

			virtual void say(DialogLine::Ptr, Actor::Ptr);

			virtual void eachFrame(uint32_t ticks) = 0;
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) = 0;

		protected:
			// map of actors to the text objects used to render
			// their dialog
			std::map<Actor::Ptr, Text::Ptr> lines;
	};

} // namespace grail

#endif // DIALOG_FRONTEND_H

