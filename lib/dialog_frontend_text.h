// vim: set noexpandtab:

#ifndef DIALOG_FRONTEND_TEXT_H
#define DIALOG_FRONTEND_TEXT_H

#include "dialog_frontend.h"

namespace grail {

	// Displays actors' lines in text floating above their heads
	class DialogFrontendText : public DialogFrontend {

		public:
			virtual void renderAt(SDL_Surface* target, uint32_t ticks, VirtualPosition p);
	};

} // namespace grail

#endif // DIALOG_FRONTEND_TEXT_H

