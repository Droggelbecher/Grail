
#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <exception>

namespace grail {

  class ValueNotSet : public std::exception {
      std::string _what;

    public:
      ValueNotSet() throw() : _what("Value not set.") {
      }

      ValueNotSet(std::string what) throw() : _what(what) {
      }

      ~ValueNotSet() throw() {
      }

      const char* what() const throw() {
        return _what.c_str();
      }
  }; // ValueNotSet

} // namespace grail

#endif // EXCEPTIONS_H

