
#include <cassert>
#include "lua_utils.h"
#include "lib/game.h"
#include "interpreter.h"
#include "lib/registry.h"
#include "lib/resource_manager.h"

//#include "lib/scene.h"


extern Interpreter interpreter;


template <typename T>
T* _LuaGet<T*>::get(lua_State* L, int n) {
  assert(lua_istable(L, n));
  lua_getfield(L, n, "_ptr");
  int top = lua_gettop(L);
  assert(lua_isuserdata(L, top));
  //T* r = dynamic_cast<T*>((Registrable*)lua_touserdata(L, top));
  T* r = (T*)lua_touserdata(L, top);
  assert(r);
  lua_pop(L, 1);
  return r;
}

template <typename T>
T& _LuaGet<T&>::get(lua_State* L, int n) {
  assert(lua_istable(L, n));
  lua_getfield(L, n, "_ptr");
  int top = lua_gettop(L);
  assert(lua_isuserdata(L, top));
  T* r = (T*)lua_touserdata(L, top);
  assert(r);
  lua_pop(L, 1);
  return *r;
}

/*
template <>
Game* _LuaGet<Game*>::get(lua_State* L, int n) {
  assert(lua_istable(L, n));
  lua_getfield(L, n, "_ptr");
  int top = lua_gettop(L);
  assert(lua_isuserdata(L, top));
  Game* r = (Game*)lua_touserdata(L, top);
  assert(r);
  lua_pop(L, 1);
  return r;
}
*/

//template <> int _LuaGet<int>::get(lua_State* L, int N) { return lua_tonumber(L, N); };
template <> lua_Number _LuaGet<lua_Number>::get(lua_State* L, int N) { return lua_tonumber(L, N); };
template <> lua_Integer _LuaGet<lua_Integer>::get(lua_State* L, int N) { return lua_tointeger(L, N); };
template <> bool _LuaGet<bool>::get(lua_State* L, int N) { return lua_toboolean(L, N); };
template <> std::string _LuaGet<std::string>::get(lua_State* L, int N) {
  size_t len;
  const char* s = lua_tolstring(L, N, &len);
  return std::string(s, len);
}
template <> VirtualPosition _LuaGet<VirtualPosition>::get(lua_State* L, int N) {
  VirtualPosition::X x;
  VirtualPosition::Y y;

  lua_getfield(L, N, "x");
  x = lua_tointeger(L, -1);
  lua_getfield(L, N, "y");
  y = lua_tointeger(L, -1);
  lua_pop(L, 2);
  return VirtualPosition(x, y);
}

template <typename T>
T luaGet(lua_State* L, int n) {
  return _LuaGet<T>::get(L, n);
}


template <typename T>
int _LuaPush<T&>::push(lua_State* L, T& obj) {
  if(!&obj) {
    lua_pushnil(L);
  }
  else {
    Object* r = dynamic_cast<Object*>(&obj);
    assert(r);
    interpreter.pushWrapper(obj);
  }
  return 1;
}

template <typename T>
int _LuaPush<T*>::push(lua_State* L, T* obj) {
  if(!obj) {
    lua_pushnil(L);
  }
  else {
    Object* r = dynamic_cast<Object*>(obj);
    assert(r);
    interpreter.pushWrapper(*obj);
  }
  return 1;
}

template <>
int _LuaPush<VirtualPosition>::push(lua_State* L, VirtualPosition obj) {
  L_STACK(L);

  lua_newtable(L);
  lua_pushinteger(L, obj.getX());
  lua_setfield(L, -2, "x");
  lua_pushinteger(L, obj.getY());
  lua_setfield(L, -2, "y");

  L_RETURN(L, 1);
  return 1;
}

template <>
int _LuaPush<const SDL_Event&>::push(lua_State* L, const SDL_Event& obj) {
  L_STACK(L);

  OmniEvent e(obj);

  lua_newtable(L);
  lua_pushinteger(L, e.getType()); lua_setfield(L, -2, "type");
  luaPush(L, e.getPosition()); lua_setfield(L, -2, "position");
  luaPush(L, e.getActor()); lua_setfield(L, -2, "actor");
  lua_pushinteger(L, e.getButton()); lua_setfield(L, -2, "button");

  L_RETURN(L, 1);
  return 1;
}

template <>
int _LuaPush<const string&>::push(lua_State* L, const string& obj) {
  L_STACK(L);
  lua_pushstring(L, obj.c_str());
  L_RETURN(L, 1);
  return 1;
}

template <>
int _LuaPush<lua_Integer>::push(lua_State* L, lua_Integer obj) {
  L_STACK(L);
  lua_pushinteger(L, obj);
  L_RETURN(L, 1);
  return 1;
}

template <typename T>
int luaPush(lua_State* L, T obj) {
  return _LuaPush<T>::push(L, obj);
}


