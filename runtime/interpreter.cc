
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "lib/utils.h"
#include "lib/shortcuts.h"
#include "lib/resource_manager.h"

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

Interpreter::~Interpreter() {
  lua_close(L);
}

void Interpreter::loadDirectory(string dir) {
  //Resource r = getResource(dir + pathDelimiter + "init.lua", MODE_READ);
  Resource r(dir + pathDelimiter + "init.lua", MODE_READ);

  size_t size;
  const char* buffer = r.createBuffer(size);
  luaL_loadbuffer(L, buffer, size, r.path.c_str());
  int error = lua_pcall(L, 0, LUA_MULTRET, 0);

  delete[] buffer;
  if(error) {
    throw lua_exception(L);
  }
}

void Interpreter::loadPrelude(string dir) {
  int r = luaL_dofile(L, (dir + pathDelimiter + "prelude.lua").c_str());
  if(r) {
    throw lua_exception(L);
  }
}

