
#include "game.h"
#include "shortcuts.h"

namespace grail {

SDL_RWops* getRW(std::string path, ResourceMode mode) {
  return Game::getInstance().getResourceManager().getRW(path, mode);
}

} // namespace grail

/*Resource getResource(std::string path, ResourceMode mode) {
  return Game::getInstance().getResourceManager().get(path, mode);
}*/

