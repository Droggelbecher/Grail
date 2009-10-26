
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>

#include <lua.hpp>

class lua_exception : public std::exception {
    lua_State* L;
  public:
    lua_exception(lua_State* L) : L(L) { }
    virtual const char* what() const throw() {
      return lua_tostring(L, -1);
    }
};


class Interpreter {
    static int l_panic(lua_State* L);
    static int l_debug(lua_State* L);

  public:
    lua_State *L;

    Interpreter();
    void loadDirectory(std::string dir);
    void loadPrelude(std::string dir);

};

#endif // INTERPRETER_H

