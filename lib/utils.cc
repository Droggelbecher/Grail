
#include <fstream>
#include <stdint.h>

#include "utils.h"

using std::list;
using std::string;
using std::ifstream;
using std::ofstream;
using std::pair;

bool startsWith(string s, string prefix) {
  return s.substr(0, prefix.length()) == prefix;
}

string lstrip(string s) {
  string::iterator iter;
  string whitespace = " \t\n";
  size_t i;
  for(i=0; i<s.length() && whitespace.find(s[i]) != string::npos; i++) {
  }
  return s.substr(i);
}

string rstrip(string s) {
  string::iterator iter;
  string whitespace = " \t\n";
  int32_t i;
  for(i=s.length() - 1; i>=0 && whitespace.find(s[i]) != string::npos; i--) {
  }
  return s.substr(0, i+1);
}

string strip(string s) { return lstrip(rstrip(s)); }

std::pair<std::string, std::string> split2(std::string s, std::string delimiter) {
  std::pair<string, string> r;
  size_t p = s.find(delimiter);
  if(p == string::npos) {
    r.first = s;
    r.second = "";
    return r;
  }

  r.first = s.substr(0, p);
  r.second = s.substr(p + 1);
  return r;
}

std::ostream& operator<<(std::ostream& os, SplitIterator& it) {
  os << "SplitIterator<" << it.s << ", " << it.delimiter << ", " << it.pos << ">";
  return os;
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


