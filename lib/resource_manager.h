
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <SDL/SDL.h>

class ResourceHandler;
class ResourceManager;


/**
 * A resource is similar to a file, however more general.
 * For example it can as well be something that only exists in RAM like a
 * decompressed gz file.
 */
class Resource {
  SDL_RWops* rw;
  public:
    Resource(SDL_RWops* rw) : rw(rw) { }
    ~Resource() { SDL_RWclose(rw); }

    /**
     * Get rwops pointer. DONT delete the returned pointer, we'll do it in our
     * d'tor.
     */
    SDL_RWops* getRW() {
      return rw;
    }
};

/**
 * Manages access to resources in a transparent way. Due to "pluggable"
 * resource handlers, you can access the contents of a zip file the same way
 * as you can access files in a real directory.
 *
 * This is similar to a unix filesystem where you can mount different
 * filesystems into the same tree. (ResourceHandlers are the anologon to file
 * system drivers here)
 */
class ResourceManager {
    std::map<std::string, ResourceHandler*> resourceHandlers;
    ResourceHandler* defaultHandler;

  public:
    enum Mode { MODE_READ = 'r', MODE_WRITE = 'w'};

    ResourceManager() : defaultHandler(0) { }
    ~ResourceManager();

    /**
     * Register given resource handler under given name.
     * Takes ownership of the given handler.
     */
    void registerHandler(std::string name, ResourceHandler* handler);

    /**
     * Caller is responsible for freeing/closing the returned RWops
     */
    SDL_RWops* getRW(std::string path, Mode mode);
    Resource
};

/**
 * Resource managers handle access to specific resource types such as zip files,
 * directories, etc...
 */
class ResourceHandler {

  public:

    ResourceHandler() { }
    virtual ~ResourceHandler() { }

    virtual SDL_RWops* get(std::string path, ResourceManager::Mode mode) = 0;
};

/**
 * Handles access to a normal directory.
 */
class DirectoryResourceHandler : public ResourceHandler {
    std::string baseDirectory;

  public:
    DirectoryResourceHandler(std::string dir);

    SDL_RWops* get(std::string path, ResourceManager::Mode mode);
};

#endif // RESOURCE_MANAGER_H

