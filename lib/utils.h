
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <list>

class Exception {
  std::string _what;
  public:
    Exception(std::string what) : _what(what) { }
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

/**
 * Return the higher of the given values
 */
template <typename T>
T max(T a, T b) { return a >= b ? a : b; }


#endif // UTILS_H

