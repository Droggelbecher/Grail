
#include <cassert>
#include "lua_utils.h"
#include "lib/game.h"
#include "interpreter.h"

extern Interpreter interpreter;

template <typename T>
T luaGet(lua_State* L, int n) {
  assert(lua_istable(L, n));
  lua_getfield(L, n, "_ptr");
  int top = lua_gettop(L);
  assert(lua_isuserdata(L, top));
  T r = (T)lua_touserdata(L, top);
  lua_pop(L, 1);
  return r;
}

template <> lua_Number luaGet<lua_Number>(lua_State* L, int N) { return lua_tonumber(L, N); };
template <> lua_Integer luaGet<lua_Integer>(lua_State* L, int N) { return lua_tointeger(L, N); };
template <> bool luaGet<bool>(lua_State* L, int N) { return lua_toboolean(L, N); };
template <> std::string luaGet<std::string>(lua_State* L, int N) {
  size_t len;
  const char* s = lua_tolstring(L, N, &len);
  return std::string(s, len);
}

void luaPushWrapper(lua_State* L, void* data, const char* className) {
  lua_getglobal(L, "_wrap_or_get");
  lua_pushlightuserdata(L, data);
  lua_pushstring(L, className);
  lua_call(L, 2, 1);
}

Game* luaGetGame() {
  lua_State* L = interpreter.L;
  lua_getglobal(L, "Game");
  return luaGet<Game*>(L, -1);
}

