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
				
				/**
				 * Takes ownership of impl
				 */
				DirectoryIterator(DirectoryIteratorImpl::Ptr impl) : impl(impl), _isEnd(false) {
					assert(impl);
				};
				
				DirectoryIterator(const DirectoryIterator& other) {
					if(other.impl) { impl = other.impl->copy(); }
					else { impl = other.impl; }
					_isEnd = other._isEnd;
					assert(_isEnd || impl);
				};
				
				~DirectoryIterator() { };
				
				DirectoryIterator& operator=(const DirectoryIterator& other) {
					if(other.impl) { impl = other.impl->copy(); }
					else { impl = other.impl; }
					_isEnd = other._isEnd;
					assert(_isEnd || impl);
					return *this;
				};
				
				DirectoryIterator& operator++() {
					++(*impl);
					return *this;
				};
				DirectoryIterator operator++(int) {
					DirectoryIterator r = *this;
					++(*impl);
					return r;
				};
				
				std::string operator*() const { return **impl; }
				
				static const DirectoryIterator& end() {
					_end._isEnd = true;
					return _end;
				}
				
				virtual bool operator==(const DirectoryIterator& other) const {
					bool this_is_end = this->_isEnd || this->impl->atEnd();
					bool other_is_end = other._isEnd || other.impl->atEnd();
					
					if(this_is_end && other_is_end) {
						return true;
					}
					
					if(this->impl && other.impl) {
						return this->impl == other.impl;
					}
					return false;
				}

				virtual bool operator!=(const DirectoryIterator& other) const {
					return !(*this == other);
				}
		}; // DirectoryIterator
		
		ResourceManager() { }
		~ResourceManager();
		
		/**
		 * Example:
		 * /media/$res/foo.png -> /media/800x600/foo.png
		 *
		 * Supported variables:
		 * $res		 ->  Physical screen resolution in format "800x600"
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
		 */
		bool exists(std::string path);
		
		/**
		 */
		DirectoryIterator beginListing(std::string path);
		
		/**
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

