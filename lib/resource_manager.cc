
#include <cassert>
#include <string>
using std::string;

#include "resource_manager.h"
#include "utils.h"

void ResourceManager::registerHandler(string name, ResourceHandler* handler) {
  resourceHandlers[name] = handler;
  defaultHandler = handler;
};

SDL_RWops* ResourceManager::get(string path, Mode mode) {
  ResourceHandler* handler = 0;

  size_t pos = path.find(':');
  if(pos == string::npos) {
    handler = defaultHandler;
  }
  else {
    string key = path.substr(0, pos);
    path = path.substr(pos+1);
    if(resourceHandlers.count(key)) {
      handler = resourceHandlers[path.substr(0, pos)];
    }
  }

  if(!handler) {
    assert(false);
    // todo ^^
    //throw std::exception("No resource handler could be found to handle '" + path + "'.");
  }
  return handler->get(path, mode);
} // get()


DirectoryResourceHandler::DirectoryResourceHandler(std::string dir) : baseDirectory(dir) {
}

SDL_RWops* DirectoryResourceHandler::get(string path, ResourceManager::Mode mode) {
  string fullpath = baseDirectory + pathDelimiter + path;

  if(mode == ResourceManager::MODE_WRITE && !exists(fullpath)) {
    touch(fullpath);
  }

  char modestring[2];
  modestring[0] = mode;
  modestring[1] = '\0';

  return SDL_RWFromFile(fullpath.c_str(), modestring);
}

