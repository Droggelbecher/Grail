
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "utils.h"

using std::string;
using std::cerr;
using std::endl;

int Interpreter::l_panic(lua_State* L) {
  cerr << "Lua error:" << lua_tostring(L, -1) << endl;
  return 0;
}

Interpreter::Interpreter() {
  L = luaL_newstate();
  lua_atpanic(L, &l_panic);
  luaL_openlibs(L);
}

void Interpreter::loadDirectory(string dir) {
  int r = luaL_dofile(L, (dir + pathDelimiter + "init.lua").c_str());
  if(r) {
    throw lua_exception(L);
  }
}

void Interpreter::loadPrelude(string dir) {
  int r = luaL_dofile(L, (dir + pathDelimiter + "prelude.lua").c_str());
  if(r) {
    throw lua_exception(L);
  }
}

