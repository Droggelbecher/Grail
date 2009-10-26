
#ifndef UTILS_H
#define UTILS_H

#include <string>


/**
 * Return true iff s starts with prefix
 */
bool startswith(std::string s, std::string prefix);

#ifdef WIN32
  const char path_delimiter = '\\';
#else
  const char path_delimiter = '/';
#endif

/**
 * Return directory name of given path
 */
std::string dirname(std::string path);

/**
 * Return the higher of the given values
 */
template <typename T>
T max(T a, T b) {
  return a >= b ? a : b;
}


#endif // UTILS_H

