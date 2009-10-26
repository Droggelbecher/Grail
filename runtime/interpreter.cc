
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "lua_value.h"
#include "utils.h"

using std::string;
using std::cerr;
using std::endl;

int Interpreter::l_panic(lua_State* L) {
  cerr << "Lua error:" << lua_tostring(L, -1) << endl;
  return 0;
}

int Interpreter::l_debug(lua_State* L) {
  int n = lua_gettop(L);
  cerr << "[debug] ";
  for(int i=1; i<=n; i++) {
    cerr << getValue(L, i) << " ";
  }
  cerr << endl;
  return 0;
}

Interpreter::Interpreter() {
  L = luaL_newstate();
  lua_atpanic(L, &l_panic);
  luaL_openlibs(L);

  lua_register(L, "debug", l_debug);

  /*
  luaL_dostring(L,
      "debug(\"halli hallo!\", 42, \"top\")"
      );
  */
}

void Interpreter::loadDirectory(string dir) {
  int r = luaL_dofile(L, (dir + path_delimiter + "init.lua").c_str());
  if(r) {
    throw lua_exception(L);
  }
}

void Interpreter::loadPrelude(string dir) {
  int r = luaL_dofile(L, (dir + path_delimiter + "prelude.lua").c_str());
  if(r) {
    throw lua_exception(L);
  }
}

