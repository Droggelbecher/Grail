// vim: set noexpandtab:

#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <map>
#include <string>
#include <cassert>
#include <boost/filesystem.hpp>

#include <SDL.h>

#include "classes.h"

namespace grail {

enum ResourceMode { MODE_READ = 'r', MODE_WRITE = 'w' };

/**
 * A resource is similar to a file, but more general.
 * For example it can as well be something that only exists in RAM like a
 * decompressed gz file.
 *
 * This class is intended for use with the RAII pattern. I.e. like:
 *
 * ----
 *
 * {
 *   Resource res("/path/to/foo.file");
 *
 *   size_t sz;
 *   char* buffer;
 *   buffer = res.createBuffer(sz);
 *
 *   // ...
 * }
 * // res is automatically destroyed at this point, the buffer however is still there
 * // if you didnt delete it yourself since it is a copy.
 *
 * ----
 */
class Resource {
		SDL_RWops* rw;
		const void* buffer;
		size_t bufferSize;
		
	private:
		// forbid copying
		Resource(const Resource& other) { }
		Resource& operator=(const Resource& other) { return *this; }
		
	public:
		/**
		 * Copy all data to a newly created buffer.
		 * Size of that buffer will be written to the size argument.
		 * The caller is responsible for freeing the allocated buffer.
		 * Do this with:
		 * ----
		 * delete[] buffer;
		 * ----
		 */
		const void* createBuffer(size_t &size);
		
		std::string path;
		
		Resource(std::string path, ResourceMode mode);
		~Resource();
		
		///
		std::string getPath() const { return path; }
		
		/**
		 * Get rwops pointer. DONT delete the returned pointer, we'll do it in our
		 * d'tor.
		 */
		SDL_RWops* getRW() { return rw; }
		
		/**
		 * Create a buffer, copy all data of the resource into it and return its
		 * address. Note that multiple calls to getData() of the same resource
		 * will return the same address, i.e. the buffer is only allocated once.
		 *
		 * Use getDataSize() in order to find out the size of the buffer.
		 * 
		 * The internal buffer will be freed when the corresponding resource
		 * object ceases to exist, i.e. you might want to copy the data.
		 */
	//	const void* getData();
		
		/**
		 * This returns the size of the buffer allocated by getDat(). If called
		 * before getData(), this will allocate the buffer and copy the data to it
		 * and getData will return its address.
		 */
	//	size_t getDataSize();
		
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
	private:
		std::map<std::string, ResourceHandler*> resourceHandlers;
		
	public:

		/**
		 */
		class DirectoryIteratorImpl {
			public:
				typedef boost::shared_ptr<DirectoryIteratorImpl> Ptr;
				virtual ~DirectoryIteratorImpl() {};
				virtual DirectoryIteratorImpl& operator++() = 0;
				virtual std::string operator*() const = 0;
				virtual bool operator==(const DirectoryIteratorImpl& other) const = 0;
				virtual DirectoryIteratorImpl::Ptr copy() const = 0;
				virtual bool atEnd() const = 0;
		};
		
		/**
		 */
		class DirectoryIterator : public std::iterator<std::input_iterator_tag, int> {
			private:
				DirectoryIteratorImpl::Ptr impl;
				bool _isEnd;
				static ResourceManager::DirectoryIterator _end;
				
			protected:
				
			public:
				/**
				 * Creates an "end"-iterator
				 */
				DirectoryIterator() : _isEnd(true) { }
				
				~DirectoryIterator() { };
				
				/**
				 * Takes ownership of impl
				 */
				DirectoryIterator(DirectoryIteratorImpl::Ptr impl);
				
				///
				DirectoryIterator(const DirectoryIterator& other);
				
				DirectoryIterator& operator=(const DirectoryIterator& other);
				
				
				static const DirectoryIterator& end();
				
				DirectoryIterator& operator++();
				DirectoryIterator operator++(int);
				std::string operator*() const;
				virtual bool operator==(const DirectoryIterator& other) const;
				virtual bool operator!=(const DirectoryIterator& other) const;
		}; // DirectoryIterator
		
		ResourceManager() { }
		virtual ~ResourceManager();
		
		/**
		 * Example:
		 * /media/$res/foo.png -> /media/800x600/foo.png
		 *
		 * Supported variables:
		 * $res -> Physical screen resolution in format "800x600"
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
		 * Return true if the giver resource exists.
		 */
		bool exists(std::string path);
		
		/**
		 * Start directory listing
		 */
		DirectoryIterator beginListing(std::string path);
		
		/**
		 * End iterator for directory listing
		 */
		DirectoryIterator endListing() { return DirectoryIterator::end(); }
		
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
		virtual ResourceManager::DirectoryIteratorImpl::Ptr beginListing(std::string path) = 0;
};

/**
 * Handles access to a normal directory.
 */
class DirectoryResourceHandler : public ResourceHandler {
		std::string baseDirectory;
		
		/**
		 */
		class DirectoryIteratorImpl : public ResourceManager::DirectoryIteratorImpl {
			private:
				boost::filesystem::directory_iterator iter;
				std::string dir;
				
			public:
				DirectoryIteratorImpl(const std::string& dir) : iter(dir), dir(dir) {
				}
				
				ResourceManager::DirectoryIteratorImpl::Ptr copy() const {
					ResourceManager::DirectoryIteratorImpl::Ptr ptr(new DirectoryIteratorImpl(dir));
					return ptr;
				}
				
				DirectoryIteratorImpl& operator++();
				std::string operator*() const { return iter->leaf(); }
				bool operator==(const ResourceManager::DirectoryIteratorImpl& other) const;
				bool atEnd() const { return iter == boost::filesystem::directory_iterator(); }
		};
		
	public:
		DirectoryResourceHandler(std::string dir);
		
		SDL_RWops* getRW(std::string path, ResourceMode mode);
		bool fileExists(std::string path);
		DirectoryIteratorImpl::Ptr beginListing(std::string path);
};

} // namespace grail

#endif // RESOURCE_MANAGER_H

