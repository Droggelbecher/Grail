// vim: set noexpandtab:

#ifndef SHORTCUTS_H
#define SHORTCUTS_H

#include <string>
#include <SDL.h>
#include "resource_manager.h"

namespace grail {

SDL_RWops* getRW(std::string path, ResourceMode mode);

Resource getResource(std::string path, ResourceMode mode);

} // namespace grail

#endif // SHORTCUTS_H

