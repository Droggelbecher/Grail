
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "lua_utils.h"
#include "lib/utils.h"
#include "lib/shortcuts.h"
#include "lib/vector2d.h"
#include "lib/resource_manager.h"
#include "lib/event.h"

using std::string;
using std::cerr;
using std::endl;

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

  size_t size;
  const char* buffer = r.createBuffer(size);
  luaL_loadbuffer(L, buffer, size, r.path.c_str());
  int error = lua_pcall(L, 0, LUA_MULTRET, 0);

  delete[] buffer;
  if(error) {
    throw LuaException(L);
  }
}

void Interpreter::loadDirectory(string dir) {
  runLuaFromResource(dir + "/init.lua");
}

void Interpreter::loadPrelude(string dir) {
  runLuaFromResource(dir + "/prelude.lua");
  /*
  int r = luaL_dofile(L, (dir + pathDelimiter + "prelude.lua").c_str());
  if(r) {
    throw LuaException(L);
  }
  */
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

void Interpreter::pushWrapper(Registrable& r) {
  L_STACK(L);

  // Lets see if there is a wrapper for r already
  pushInternalTable();
  getOrCreateAsEmptyTable("wrappers");
  // [ internal | wrappers ]
  lua_remove(L, -2); // remove internal table
  // [ wrappers ]

  size_t s_wrappers = lua_gettop(L);

  lua_pushlightuserdata(L, &r);
  // [ wrappers | &r ]
  lua_gettable(L, s_wrappers);
  // [ wrappers | wrapper/nil ]

  if(lua_isnil(L, -1)) {
    lua_pop(L, 1);
    lua_newtable(L);
    // [ wrappers | new wrap ]
    L_RETURN(L, 2);

    size_t s_new_wrap = lua_gettop(L);
    assert(lua_type(L, s_new_wrap) == LUA_TTABLE);

    lua_pushlightuserdata(L, &r); // key for insertion into s_wrappers
    // [ wrappers | new wrap | &r ]
    L_RETURN(L, 3);

    lua_pushvalue(L, s_new_wrap);
    // [ wrappers | new wrap | &r | new wrap ]
    L_RETURN(L, 4);

    lua_pushlightuserdata(L, &r);
    // [ wrappers | new wrap | &r | new wrap | &r ]
    L_RETURN(L, 5);

    lua_setfield(L, s_new_wrap, "_ptr");
    // [ wrappers | new wrap | &r | new wrap ]
    L_RETURN(L, 4);

    pushWrapperBase(r.className); L_NOTNIL(L);
    // [ wrappers | new wrap | &r | new wrap | base ]
    L_RETURN(L, 5);

    lua_setmetatable(L, s_new_wrap);
    // [ wrappers | new wrap | &r | new wrap ]
    L_RETURN(L, 4);

    // insert wrapper into "wrappers"
    lua_settable(L, s_wrappers);
    // [ wrappers | new wrap ]
    L_RETURN(L, 2);
  }

  // [ wrappers | new wrap ]
  L_RETURN(L, 2);
  lua_remove(L, s_wrappers); // remove "wrappers" table

  L_RETURN(L, 1);
}


template <typename T>
void Interpreter::pushCopy(T* obj) {
  if(obj) {
    pushWrapper(*((Registrable*)obj));
  }
  else {
    lua_pushnil(L);
  }
}

template <typename T>
void Interpreter::pushCopy(T obj) {
  obj.__type_supported_by_interpreter_pushCopy();
}

template <>
void Interpreter::pushCopy(lua_Integer obj) { lua_pushinteger(L, obj); }

template <>
void Interpreter::pushCopy(VirtualPosition p) {
  L_STACK(L);
  lua_newtable(L);
  pushCopy((lua_Integer)p.getX()); lua_setfield(L, -2, "x");
  pushCopy((lua_Integer)p.getY()); lua_setfield(L, -2, "y");
  L_RETURN(L, 1);
}


template <>
void Interpreter::pushCopy(SDL_Event& obj) {
  L_STACK(L);

  OmniEvent e(obj);

  lua_newtable(L);
  pushCopy((lua_Integer)e.getType()); lua_setfield(L, -2, "type");
  pushCopy(e.getPosition()); lua_setfield(L, -2, "position");
  pushCopy(e.getActor()); lua_setfield(L, -2, "actor");
  pushCopy((lua_Integer)e.getButton()); lua_setfield(L, -2, "button");

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

