
#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <string>
#include <lua.hpp>

/**
 * luaGet<Type, int N>(lua_State* L)
 *
 * Get data from stack at position N as type Type
 */

// Default: Assume T is a class pointer
template <typename T> extern T luaGet(lua_State* L, int n);
template <> lua_Number luaGet<lua_Number>(lua_State* L, int n);
template <> lua_Integer luaGet<lua_Integer>(lua_State* L, int n);
template <> bool luaGet<bool>(lua_State* L, int n);
template <> std::string luaGet<std::string>(lua_State* L, int N);

#define BEGIN_LUA_STACK_ASSERTION() int __stack_assert = lua_gettop(L)
#define END_LUA_STACK_ASSERTION(N) assert(lua_gettop(L) == (__stack_assert + (N)))

#endif // LUA_UTILS_H

