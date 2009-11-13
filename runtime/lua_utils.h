
#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <string>
#include <lua.hpp>
#include "lib/vector2d.h"

/**
 * luaGet<Type, int N>(lua_State* L)
 *
 * Get data from stack at position N as type Type
 */

// Use a struct with a single method because classes/struct templates can be partially
// specialized, function templates can not
// (Idea from: http://www.gotw.ca/publications/mill17.htm)
template <typename T> struct _LuaGet { static T get(lua_State* L, int n); };
template <typename T> struct _LuaGet<T*> { static T* get(lua_State* L, int n); };
template <typename T> struct _LuaGet<T&> { static T& get(lua_State* L, int n); };

template <typename T> T luaGet(lua_State* L, int n);


template <typename T> struct _LuaPush { static int push(lua_State* L, T obj); };
template <typename T> struct _LuaPush<T*> { static int push(lua_State* L, T* obj); };
template <typename T> struct _LuaPush<T&> { static int push(lua_State* L, T& obj); };
/*template <> struct _LuaPush<VirtualPosition> { static int push(lua_State* L, VirtualPosition obj); };
template <> struct _LuaPush<const SDL_Event&> { static int push(lua_State* L, const SDL_Event& obj); };
template <> struct _LuaPush<const std::string&> { static int push(lua_State* L, const std::string& obj); };
template <> struct _LuaPush<lua_Integer> { static int push(lua_State* L, lua_Integer obj); };
*/
template <typename T> int luaPush(lua_State* L, T obj);

#define L_STACK(L) int __lua_stack_assert = lua_gettop(L);
#define L_RETURN(L, N) assert(lua_gettop(L) == (__lua_stack_assert + (N)));
#define L_NOTNIL(L) assert(!lua_isnil(L, -1))

#endif // LUA_UTILS_H

