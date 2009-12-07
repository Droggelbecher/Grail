
#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <lua.hpp>
#include "lib/registry.h"

class LuaException : public std::exception {
    lua_State* L;
  public:
    LuaException(lua_State* L) : L(L) { }
    virtual const char* what() const throw() {
      return lua_tostring(L, -1);
    }
};


class Interpreter {
    static int l_panic(lua_State* L);
    static const std::string internalTableName;

    static int l_doresource(lua_State* L);

    void pushInternalTable();
    void getOrCreateAsEmptyTable(std::string fieldName);
    void makeBase();

  public:
    lua_State *L;

    Interpreter();
    ~Interpreter();

    void runLuaFromResource(std::string path);
    void loadDirectory(std::string dir);
    void loadPrelude(std::string dir);

    void pushWrapperBase(std::string className);
    void pushWrapper(Object& r);

    void registerFunction(std::string name, lua_CFunction fn);
    void registerBase(std::string baseName);
    void registerMethod(std::string baseName, std::string name, lua_CFunction fn);
};

extern Interpreter interpreter;

#endif // INTERPRETER_H

