// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_H
#define DIALOG_FRONTEND_H

#include <boost/shared_ptr.hpp>
#include <list>

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
			std::list<DialogLine::Ptr> lines;

		public:
			typedef boost::shared_ptr<DialogFrontend> Ptr;

			virtual void eachFrame(uint32_t ticks);
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) = 0;

			/*
 			* collects all the new dialog lines from a scene
 			*/
			std::list<DialogLine::Ptr> updateDialogLines();
	};

} // namespace grail

#endif // DIALOG_FRONTEND_H

