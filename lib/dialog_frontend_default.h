// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_DEFAULT_H
#define DIALOG_FRONTEND_DEFAULT_H

#include "dialog_frontend.h"

namespace grail {

	// The provisional default dialog renderer
	// This could be maybe coloured text floating above an actor's head.
	// NOTE:: for now all this does is print the dialog to the output. :)
	class DialogFrontendDefault : public DialogFrontend {

		public:
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p);
	};

} // namespace grail

#endif // DIALOG_FRONTEND_DEFAULT_H

