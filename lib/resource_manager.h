
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <cassert>

#include <SDL/SDL.h>

#include "classes.h"

namespace grail {

enum ResourceMode { MODE_READ = 'r', MODE_WRITE = 'w' };

/**
 * A resource is similar to a file, however more general.
 * For example it can as well be something that only exists in RAM like a
 * decompressed gz file.
 */
class Resource {
    SDL_RWops* rw;
    const char* buffer;
    size_t bufferSize;

  private:
    // forbid copying
    Resource(const Resource& other) { }
    Resource& operator=(const Resource& other) { return *this; }

    /**
     * Copy all data to a newly created buffer.
     * Size of that buffer will be written to the size argument.
     */
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


  public:
    std::string path;

    Resource(std::string path, ResourceMode mode);
    ~Resource();

    std::string getPath() const { return path; }

    /**
     * Get rwops pointer. DONT delete the returned pointer, we'll do it in our
     * d'tor.
     */
    SDL_RWops* getRW() {
      return rw;
    }

    const char* getData() {
      if(!buffer) {
        buffer = createBuffer(bufferSize);
      }
      return buffer;
    }

    size_t getDataSize() {
      if(!buffer) {
        buffer = createBuffer(bufferSize);
      }
      return bufferSize;
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
class ResourceManager {
    std::map<std::string, ResourceHandler*> resourceHandlers;

  public:
    static const std::string className;

    ResourceManager() { }
    ~ResourceManager();


    /**
     * Example:
     * /media/$res/foo.png -> /media/800x600/foo.png
     *
     * Supported variables:
     * $res    ->  Physical screen resolution in format "800x600"
     */
    std::string substituteVariables(std::string path);

    /**
     * Register given resource handler under given name.
     * Takes ownership of the given handler. (Ie you should not delete it
     * after calling this method)
     */
    void mount(ResourceHandler* handler, std::string path);

    /**
     */
    ResourceHandler* findHandler(std::string path, std::string &mountpoint);

    /**
     * Caller is responsible for freeing/closing the returned RWops
     */
    SDL_RWops* getRW(std::string path, ResourceMode mode);
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
    virtual bool fileExists(std::string path) = 0;
    //virtual std::list<std::string> listDirectory(std::string path) = 0;
};

/**
 * Handles access to a normal directory.
 */
class DirectoryResourceHandler : public ResourceHandler {
    std::string baseDirectory;

  public:
    DirectoryResourceHandler(std::string dir);

    SDL_RWops* getRW(std::string path, ResourceMode mode);
    bool fileExists(std::string path);
    //std::list<std::string> listDirectory(std::string path);
};

} // namespace grail

#endif // RESOURCE_MANAGER_H

