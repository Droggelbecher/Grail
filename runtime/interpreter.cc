
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "lua_utils.h"
#include "lib/utils.h"
#include "lib/shortcuts.h"
#include "lib/vector2d.h"
#include "lib/resource_manager.h"
#include "lib/event.h"
#include "lib/debug.h"

using std::string;
using std::cerr;
using std::endl;
using namespace grail;

const std::string Interpreter::internalTableName = "_GRAIL";

void Interpreter::getOrCreateAsEmptyTable(std::string fieldName) {
  L_STACK(L);

  lua_getfield(L, -1, fieldName.c_str());
  if(lua_isnil(L, -1)) {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setfield(L, -3, fieldName.c_str());
  }

  L_NOTNIL(L);
  L_RETURN(L, 1);
}

void Interpreter::makeBase() {
  L_STACK(L);
  lua_getfield(L, -1, "__index");
  if(lua_isnil(L, -1)) {
    lua_pop(L, 1);
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");
  }
  else {
    lua_pop(L, 1);
  }
  L_RETURN(L, 0);
}

int Interpreter::l_panic(lua_State* L) {
  throw LuaException(L);
  return 0;
}

Interpreter::Interpreter() {
  L = luaL_newstate();
  lua_atpanic(L, &l_panic);
  luaL_openlibs(L);

  lua_register(L, "doresource", &l_doresource);
}

Interpreter::~Interpreter() {
  lua_close(L);
}

int Interpreter::l_doresource(lua_State* L) {
  const char* p = lua_tostring(L, 1);
  interpreter.runLuaFromResource(string(p));
  return 0;
}

void Interpreter::runLuaFromResource(string path) {
  Resource r(path, MODE_READ);

  luaL_loadbuffer(L, r.getData(), r.getDataSize(), r.path.c_str());
  int error = lua_pcall(L, 0, LUA_MULTRET, 0);

  if(error) {
    throw LuaException(L);
  }
}

void Interpreter::loadDirectory(string dir) {
  runLuaFromResource(dir + "/init.lua");
}

void Interpreter::loadPrelude(string dir) {
  runLuaFromResource(dir + "/prelude.lua");
}

void Interpreter::pushInternalTable() {
  L_STACK(L);

  lua_getglobal(L, internalTableName.c_str());
  if(lua_isnil(L, -1)) {
    lua_pop(L, 1);
    lua_newtable(L);
    lua_pushvalue(L, -1);
    lua_setglobal(L, internalTableName.c_str());
  }

  L_RETURN(L, 1);
}

void Interpreter::pushWrapperBase(std::string className) {
  L_STACK(L);

  pushInternalTable();
  getOrCreateAsEmptyTable("wrapperBases");
  getOrCreateAsEmptyTable(className);
  makeBase();

  lua_remove(L, -3); // internalTable
  lua_remove(L, -2); // wrapperBases

  L_RETURN(L, 1);
}

void Interpreter::registerFunction(std::string name, lua_CFunction fn) {
  L_STACK(L);

  //lua_getglobal(L, internalTableName.c_str());
  pushInternalTable();
  lua_pushcfunction(L, fn);
  lua_setfield(L, -2, name.c_str());
  lua_pop(L, 1);

  L_RETURN(L, 0);
}

void Interpreter::registerBase(string baseName) {
  L_STACK(L);
  pushWrapperBase(baseName);
  lua_pop(L, 1);
  L_RETURN(L, 0);
}

void Interpreter::registerMethod(string baseName, string name, lua_CFunction fn) {
  L_STACK(L);

  pushWrapperBase(baseName);
  lua_pushcfunction(L, fn);
  lua_setfield(L, -2, name.c_str());
  lua_pop(L, 1);

  L_RETURN(L, 0);
}

