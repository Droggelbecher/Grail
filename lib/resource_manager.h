
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <cassert>

#include <SDL/SDL.h>

#include "classes.h"
#include "registry.h"

enum ResourceMode { MODE_READ = 'r', MODE_WRITE = 'w' };

/**
 * A resource is similar to a file, however more general.
 * For example it can as well be something that only exists in RAM like a
 * decompressed gz file.
 */
class Resource {
    SDL_RWops* rw;

  private:
    // forbid copying
    /*Resource(const Resource& other) {
      rw = other.rw;
      path = other.path;
    }*/
    Resource& operator=(const Resource& other) {
      rw = other.rw;
      path = other.path;
      return *this;
    }

  public:
    std::string path;

    Resource(SDL_RWops* rw, std::string path = "") : rw(rw), path(path) { }
    ~Resource() { SDL_RWclose(rw); }

    /**
     * Get rwops pointer. DONT delete the returned pointer, we'll do it in our
     * d'tor.
     */
    SDL_RWops* getRW() {
      return rw;
    }

    const char* createBuffer(size_t &size) {
      assert(rw);

      char* buffer;
      SDL_RWseek(rw, 0, SEEK_END);
      size = SDL_RWtell(rw);
      SDL_RWseek(rw, 0, SEEK_SET);
      buffer = new char[size];
      size_t read = 0;
      while(read < size) {
        read += SDL_RWread(rw, buffer, sizeof(char), size - read);
      }
      return buffer;
    }

    friend class ResourceManager;
    friend Resource getResource(std::string, ResourceMode);
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
class ResourceManager : public Registrable {
    std::map<std::string, ResourceHandler*> resourceHandlers;

  public:
    ResourceManager() : Registrable("ResourceManager") { }
    ~ResourceManager();

    /**
     * Register given resource handler under given name.
     * Takes ownership of the given handler. (Ie you should not delete it
     * after calling this method)
     */
    //void registerHandler(std::string name, ResourceHandler* handler);
    void mount(ResourceHandler* handler, std::string path);

    /**
     */
    ResourceHandler* findHandler(std::string path, std::string &mountpoint);

    /**
     * Caller is responsible for freeing/closing the returned RWops
     */
    SDL_RWops* getRW(std::string path, ResourceMode mode);

    /**
     */
    Resource get(std::string path, ResourceMode mode) { return Resource(getRW(path, mode), path); }
};

/**
 * Resource managers handle access to specific resource types such as zip files,
 * directories, etc...
 */
class ResourceHandler {

  public:

    ResourceHandler() { }
    virtual ~ResourceHandler() { }

    virtual SDL_RWops* getRW(std::string path, ResourceMode mode) = 0;
};

/**
 * Handles access to a normal directory.
 */
class DirectoryResourceHandler : public ResourceHandler {
    std::string baseDirectory;

  public:
    DirectoryResourceHandler(std::string dir);

    SDL_RWops* getRW(std::string path, ResourceMode mode);
};

#endif // RESOURCE_MANAGER_H

