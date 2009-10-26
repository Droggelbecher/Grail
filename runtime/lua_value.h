
#include <iostream>
#include <lua.hpp>
#include <cassert>

struct LuaValue {
  uint8_t type;
  lua_Number number;
  std::string string;
};

std::ostream& operator<<(std::ostream& os, const LuaValue v) {
  switch(v.type) {
    case LUA_TSTRING:
      os << v.string;
      break;
    case LUA_TNUMBER:
      os << v.number;
      break;
  }
  return os;
};

LuaValue getValue(lua_State* L, int i) {
  LuaValue r;
  const char* s;
  size_t len;

  r.type = lua_type(L, i);

  switch(r.type) {
    case LUA_TNUMBER:
      r.number = lua_tonumber(L, i);
      break;

    case LUA_TSTRING:
      s = lua_tolstring(L, i, &len);
      r.string = std::string(s, len);
      break;

    default:
      assert(false);
      break;
  }
  return r;
}


