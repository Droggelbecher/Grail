// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_H
#define DIALOG_FRONTEND_H

#include <boost/shared_ptr.hpp>
#include "dialog_line.h"

namespace grail {

	// Virtual base class for in-game dialog renderers.
	// Intended to be pluggable so a developer may customise how dialog
	// is outputted to the screen.
	// For example via subtitles or text floating above an actor's head.
	class DialogFrontend {

		public:
			typedef boost::shared_ptr<DialogFrontend> Ptr;

			// renders the dialog line
			virtual void render(DialogLine::Ptr) = 0;
	};

} // namespace grail

#endif // DIALOG_FRONTEND_H

