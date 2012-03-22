// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_H
#define DIALOG_FRONTEND_H

#include <boost/shared_ptr.hpp>
#include <map>

#include "dialog_line.h"
#include "actor.h"
#include "text.h"

namespace grail {

	/**
	* Virtual base class for in-game dialog renderers,
	* which arentended to be pluggable so a developer may customise how dialog
	* is outputted to the screen, eg. subtitles, floating text
	*/
	class DialogFrontend {

		protected:
			// map of actors to the text objects used to render
			// their dialog
			std::map<Actor::Ptr, Text::Ptr> lines;

		public:
			typedef boost::shared_ptr<DialogFrontend> Ptr;

			virtual void say(DialogLine::Ptr) = 0;

			virtual void eachFrame(uint32_t ticks) = 0;
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) = 0;

	};

} // namespace grail

#endif // DIALOG_FRONTEND_H

