
#include <string>
#include <fstream>

#include "utils.h"

bool startswith(std::string s, std::string prefix) {
  return s.substr(0, prefix.length()) == prefix;
}

std::string dirname(std::string path) {
  size_t pos = path.find_last_of('/');
  if(pos == std::string::npos) {
    return ".";
  }
  else {
    return path.substr(0, pos);
  }
}

void touch(std::string path) {
  std::ofstream s(path.c_str());
}

bool exists(std::string path) {
  std::ifstream s(path.c_str());
  return (bool)s;
}

