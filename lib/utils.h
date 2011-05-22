// vim: set noexpandtab:

#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <exception>
#include <iostream>
#include <list>
#include <sstream>
#include <stdint.h>
#include <string>

namespace grail {

class Exception : public std::exception {
	std::string _what;
	public:
		Exception(std::string what) throw() : _what(what) { }
		virtual ~Exception() throw() { }
		virtual const char* what() const throw() { return _what.c_str(); }
};

//
// String stuff
//

/**
 * Strip white spaces on the left side
 */
std::string lstrip(std::string s);

/**
 * Strip white spaces on the right side
 */
std::string rstrip(std::string s);


/**
 * Strip white spaces on both sides
 */
std::string strip(std::string s);

/**
 * Split given string in two at the given delimiter.
 */
std::pair<std::string, std::string> split2(std::string s, std::string delimiter = "=");

class SplitIterator;

std::ostream& operator<<(std::ostream&, SplitIterator&);

/**
 * Iterator over split "words"
 */
class SplitIterator : public std::iterator<std::input_iterator_tag, std::string> {
		std::string s;
		std::string delimiter;
		size_t pos;

	public:
		SplitIterator(std::string s, std::string delimiter, size_t pos=0) : s(s), delimiter(delimiter), pos(pos) {
		}
		SplitIterator(const SplitIterator& other) : s(other.s), delimiter(other.delimiter), pos(other.pos) {
		}
		
		static SplitIterator end(std::string s, std::string delimiter)	{
			return SplitIterator(s, delimiter, std::string::npos);
		}
		
		SplitIterator end() {
			return SplitIterator(s, delimiter, std::string::npos);
		}
		
		SplitIterator& operator++() {
			pos = s.find(delimiter, pos);
			if(pos != std::string::npos) pos++;
			return *this;
		}
		
		SplitIterator& operator++(int) {
			SplitIterator& r(*this);
			pos = s.find(delimiter, pos);
			if(pos != std::string::npos) pos++;
			return r;
		}
		
		bool operator==(const SplitIterator& other) {
			return s == other.s && delimiter == other.delimiter && pos == other.pos;
		}
		
		bool operator!=(const SplitIterator& other) {
			return !(*this == other);
		}
		
		std::string operator*() const {
			assert(pos != std::string::npos);
			size_t p = s.find(delimiter, pos);
			if(p == std::string::npos) {
				p = s.length();
			}
			return s.substr(pos, p-pos);
		}
		
		friend std::ostream& operator<<(std::ostream&, SplitIterator&);
}; // class SplitIterator



/**
 * Return true iff s starts with prefix
 */
bool startsWith(std::string s, std::string prefix);

template <typename T>
T fromString(std::string s) {
	std::istringstream ss(s);
	T r;
	ss >> r;
	return r;
}

template <typename T>
std::string toString(T o) {
	std::ostringstream ss;
	ss << o;
	return ss.str();
}

//
// File(name) handling
//

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
 * Return a version of path that fullfills the following constrains:
 * - The new path starts with a "/"
 * - The new path does not end with a "/" (except if it is exactly "/")
 * - There are no repetitions of "/"s
 */
std::string normalizePath(std::string path);

/**
 * Return true iff path1 is a parent path or logically equal to path2.
 * If that is so, sub is filled with the subpath that leads from path1 to
 * path2
 */
bool isParentOrEqualPath(std::string path1, std::string path2);

//
// Math stuff
//

/**
 * Given an integer n, return the next power of 2  that is >= n.
 */
uint16_t nextPower2(uint16_t n);

} // namespace grail

#endif // UTILS_H

