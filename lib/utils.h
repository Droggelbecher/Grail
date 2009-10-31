
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>
#include <exception>

class Exception : public std::exception {
  std::string _what;
  public:
    Exception(std::string what) throw() : _what(what) { }
    virtual ~Exception() throw() { }
    virtual const char* what() const throw() { return _what.c_str(); }
};


//
// File(name) handling
//

/**
 * Return true iff s starts with prefix
 */
bool startsWith(std::string s, std::string prefix);

#ifdef WIN32
  const char pathDelimiter = '\\';
#else
  const char pathDelimiter = '/';
#endif

/**
 * Return directory name of given path
 */
std::string dirName(std::string path);

/**
 */
void touch(std::string path);

/**
 */
bool exists(std::string path);

/**
 */
bool isAbsolute(std::string path);

/**
 */
std::string normalizePath(std::string path);

/**
 * Return true iff path1 is a parent path or logically equal to path2.
 * If that is so, sub is filled with the subpath that leads from path1 to
 * path2
 */
bool isParentOrEqualPath(std::string path1, std::string path2);

#endif // UTILS_H

