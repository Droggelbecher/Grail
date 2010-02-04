
#include <cassert>
#include <sstream>
using std::ostringstream;
#include <string>
using std::string;
#include <map>
using std::map;
#include <iostream>
using std::cerr;
using std::endl;

#include "resource_manager.h"
#include "utils.h"
#include "game.h"
#include "viewport.h"
#include "debug.h"

namespace grail {

Resource::Resource(string path, ResourceMode mode) : buffer(0), bufferSize(0), path(path) {
  rw = Game::getInstance().getResourceManager().getRW(path, mode);
}

Resource::~Resource() {
  SDL_RWclose(rw);
  delete[] buffer;
}


const std::string ResourceManager::className = "ResourceManager";

ResourceManager::~ResourceManager() {
  map<string, ResourceHandler*>::const_iterator iter;

  for(iter = resourceHandlers.begin(); iter != resourceHandlers.end(); iter++) {
    delete iter->second;
  }
  resourceHandlers.clear();
}

string ResourceManager::substituteVariables(string path) {
  size_t p = 0;
  ostringstream ss;

  while(true) {
    p = path.find("$res");
    if(p == string::npos) {
      break;
    }

    ss.clear();
    ss << Game::getInstance().getViewport().getPhysicalWidth() << "x"
      << Game::getInstance().getViewport().getPhysicalHeight();
    path.replace(p, 4, ss.str());
  }
  return path;
}

void ResourceManager::mount(ResourceHandler* handler, string path) {
  assert(isAbsolute(path));
  resourceHandlers[normalizePath(path)] = handler;
}

ResourceHandler* ResourceManager::findHandler(string path, string& mountpoint) {
  map<string, ResourceHandler*>::const_iterator iter;
  string sub;

  path = normalizePath(path);

  for(iter = resourceHandlers.begin(); iter != resourceHandlers.end(); iter++) {
    mountpoint = normalizePath(iter->first);
    sub = path.substr(mountpoint.length());
    if(isParentOrEqualPath(iter->first, path) && iter->second->fileExists(sub)) {
      return iter->second;
    }
  }
  return 0;
}

SDL_RWops* ResourceManager::getRW(string path, ResourceMode mode) {
  path = normalizePath(substituteVariables(path));

  string mountpoint;
  ResourceHandler* handler = findHandler(path, mountpoint);

  if(!handler) {
    throw Exception(
        string("No resource handler could be found for \"") + path + string("\".")
    );
  }

  return handler->getRW(path.substr(mountpoint.length()), mode);
} // getRW()


DirectoryResourceHandler::DirectoryResourceHandler(std::string dir) : baseDirectory(dir) {
}

SDL_RWops* DirectoryResourceHandler::getRW(string path, ResourceMode mode) {
  string fullpath = baseDirectory + pathDelimiter + path;

  if(mode == MODE_WRITE && !exists(fullpath)) {
    touch(fullpath);
  }

  char modestring[2];
  modestring[0] = mode;
  modestring[1] = '\0';

  SDL_RWops* r = SDL_RWFromFile(fullpath.c_str(), modestring);
  if(!r) {
    throw Exception(
        string("Could not load '") + path + string("' (-> ") + fullpath + string(")")
        );
  }
  return r;
}

bool DirectoryResourceHandler::fileExists(string path) {
  string fullpath = baseDirectory + pathDelimiter + path;
  return exists(fullpath);
}

} // namespace grail


