
#include <fstream>

#include "utils.h"

using std::list;
using std::string;
using std::ifstream;
using std::ofstream;

bool startsWith(string s, string prefix) {
  return s.substr(0, prefix.length()) == prefix;
}

//
// File stuff
//

string dirName(string path) {
  size_t pos = path.find_last_of('/');
  if(pos == string::npos) {
    return ".";
  }
  else {
    return path.substr(0, pos);
  }
}

void touch(std::string path) {
  ofstream s(path.c_str());
}

bool exists(string path) {
  ifstream s(path.c_str());
  return (bool)s;
}

bool isAbsolute(string path) {
  return (path.length() > 0) && (path.at(0) == pathDelimiter);
}

string normalizePath(string path) {
  string::const_iterator iter;
  bool delimiterRead = true;
  string r(1, pathDelimiter);

  for(iter = path.begin(); iter != path.end(); iter++) {
    if(*iter == pathDelimiter) {
      if(!delimiterRead) {
        r.append(1, pathDelimiter);
        delimiterRead = true;
      }
    }
    else {
      r.append(1, *iter);
      delimiterRead = false;
    }
  }

  if(r.at(r.length() - 1) == pathDelimiter) {
    r.resize(r.length() - 1);
  }
  return r;
}

bool isParentOrEqualPath(string path1, string path2) {
  return path1 == path2.substr(0, path1.length());
}


