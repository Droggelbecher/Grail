
#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>

#include <SDL/SDL.h>

class ResourceHandler;
class ResourceManager;

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

    SDL_RWops* get(std::string path, Mode mode);
};

class ResourceHandler {

  public:

    ResourceHandler() { }
    virtual ~ResourceHandler() { }

    virtual SDL_RWops* get(std::string path, ResourceManager::Mode mode) = 0;
};

class DirectoryResourceHandler : public ResourceHandler {
    std::string baseDirectory;

  public:
    DirectoryResourceHandler(std::string dir);

    SDL_RWops* get(std::string path, ResourceManager::Mode mode);
};

#endif // RESOURCE_MANAGER_H

