// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_H
#define DIALOG_FRONTEND_H

#include <boost/shared_ptr.hpp>
#include <list>

#include "dialog_line.h"
#include "actor.h"
#include "user_interface_element.h"

namespace grail {

	/**
	* Virtual base class for in-game dialog renderers,
	* which arentended to be pluggable so a developer may customise how dialog
	* is outputted to the screen, eg. subtitles, floating text
	*/
	class DialogFrontend : public UserInterfaceElement {
		protected:
			/*
 			* dialog lines being spoken in the scene
 			*/
			std::list<DialogLine::Ptr> lines;

		public:
			typedef boost::shared_ptr<DialogFrontend> Ptr;

			virtual VirtualPosition getSize() const { }

			virtual void eachFrame(uint32_t ticks);
			virtual EventState handleEvent(const SDL_Event& event, uint32_t frameDuration) {};
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p) const = 0;

			/*
 			* collects all the new dialog lines from a scene and adds
 			* them to the list
 			*/
			std::list<DialogLine::Ptr> updateDialogLines();
	};

} // namespace grail

#endif // DIALOG_FRONTEND_H

