// vim: set noexpandtab:

#include "dialog_frontend_default.h"

namespace grail {

	// for now just prints out the dialog to output
	void DialogFrontendDefault::render(DialogLine::Ptr dialogLine) {
		printf("%s\n", dialogLine->getText().c_str()); 
	}

} // namespace grail

