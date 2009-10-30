
#include "game.h"
#include "shortcuts.h"

SDL_RWops* getRW(std::string path, ResourceMode mode) {
  return Game::getInstance().getResourceManager().getRW(path, mode);
}

Resource getResource(std::string path, ResourceMode mode) {
  return Game::getInstance().getResourceManager().get(path, mode);
}

