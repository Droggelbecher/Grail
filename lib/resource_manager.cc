// vim: set noexpandtab:

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

const void* Resource::createBuffer(size_t &size) {
	assert(rw);
	assert(sizeof(unsigned char) == sizeof(signed char));
	
	unsigned char* buffer;
	SDL_RWseek(rw, 0, SEEK_END);
	size = SDL_RWtell(rw);
	SDL_RWseek(rw, 0, SEEK_SET);
	buffer = new unsigned char[size];
	size_t read = 0;
	while(read < size) {
		read += SDL_RWread(rw, buffer, sizeof(unsigned char), size - read);
	}
	return buffer;
}

const void* Resource::getData() {
	if(!buffer) {
		buffer = createBuffer(bufferSize);
	}
	return buffer;
}

size_t Resource::getDataSize() {
	if(!buffer) {
		buffer = createBuffer(bufferSize);
	}
	return bufferSize;
}

Resource::Resource(string path, ResourceMode mode) : buffer(0), bufferSize(0), path(path) {
	rw = Game::getInstance().getResourceManager().getRW(path, mode);
}

Resource::~Resource() {
	SDL_RWclose(rw);
	delete[] static_cast<const unsigned char*>(buffer);
}

//
// ResourceManager::DirectoryIterator
//

ResourceManager::DirectoryIterator ResourceManager::DirectoryIterator::_end;

ResourceManager::DirectoryIterator::DirectoryIterator(ResourceManager::DirectoryIteratorImpl::Ptr impl) : impl(impl), _isEnd(false) {
	assert(impl);
};

ResourceManager::DirectoryIterator::DirectoryIterator(const ResourceManager::DirectoryIterator& other) {
	if(other.impl) { impl = other.impl->copy(); }
	else { impl = other.impl; }
	_isEnd = other._isEnd;
	assert(_isEnd || impl);
};

ResourceManager::DirectoryIterator& ResourceManager::DirectoryIterator::operator=(const ResourceManager::DirectoryIterator& other) {
	if(other.impl) { impl = other.impl->copy(); }
	else { impl = other.impl; }
	_isEnd = other._isEnd;
	assert(_isEnd || impl);
	return *this;
};

ResourceManager::DirectoryIterator& ResourceManager::DirectoryIterator::operator++() {
	++(*impl);
	return *this;
};

ResourceManager::DirectoryIterator ResourceManager::DirectoryIterator::operator++(int) {
	ResourceManager::DirectoryIterator r = *this;
	++(*impl);
	return r;
};

std::string ResourceManager::DirectoryIterator::operator*() const { return **impl; }

const ResourceManager::DirectoryIterator& ResourceManager::DirectoryIterator::end() {
	_end._isEnd = true;
	return _end;
}

bool ResourceManager::DirectoryIterator::operator==(const ResourceManager::DirectoryIterator& other) const {
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

bool ResourceManager::DirectoryIterator::operator!=(const ResourceManager::DirectoryIterator& other) const {
	return !(*this == other);
}

//
// ResourceManager
//


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
		if(mountpoint.length() <= path.length()) {
			sub = path.substr(mountpoint.length());
			if(isParentOrEqualPath(iter->first, path) && iter->second->fileExists(sub)) {
				return iter->second;
			}
		}
	}
	return 0;
}

bool ResourceManager::exists(string path) {
	path = normalizePath(substituteVariables(path));
	string mountpoint;
	ResourceHandler* handler = findHandler(path, mountpoint);
	return (handler != 0);
}

ResourceManager::DirectoryIterator ResourceManager::beginListing(string path) {
	path = normalizePath(substituteVariables(path));
	string mountpoint;
	ResourceHandler* handler = findHandler(path, mountpoint);
	if(!handler) {
		return endListing();
	}
	return DirectoryIterator(handler->beginListing(path));
} // beginListing

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


//
//
//

DirectoryResourceHandler::DirectoryResourceHandler(string dir) : baseDirectory(dir) {
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

DirectoryResourceHandler::DirectoryIteratorImpl::Ptr DirectoryResourceHandler::beginListing(std::string path) {
	string fullpath = baseDirectory + pathDelimiter + path;
	return ResourceManager::DirectoryIteratorImpl::Ptr(new DirectoryIteratorImpl(fullpath));
}

//
//
//

DirectoryResourceHandler::DirectoryIteratorImpl& DirectoryResourceHandler::DirectoryIteratorImpl::operator++() {
	++iter;
	return *this;
}

bool DirectoryResourceHandler::DirectoryIteratorImpl::operator==(const ResourceManager::DirectoryIteratorImpl& other) const {
	const DirectoryIteratorImpl* o = dynamic_cast<const DirectoryIteratorImpl*>(&other);
	if(!o) {
		return false;
	}
	return iter == o->iter;
}

} // namespace grail


