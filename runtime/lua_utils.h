
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

/*
#define BEGIN_LUA_STACK_ASSERTION(L) int __stack_assert = lua_gettop(L)
#define END_LUA_STACK_ASSERTION(L, N) assert(lua_gettop(L) == (__stack_assert + (N)))
#define LUA_ASSERT_NOT_NIL(L) assert(!lua_isnil(L))
*/

#define L_STACK(L) int __lua_stack_assert = lua_gettop(L);
#define L_RETURN(L, N) assert(lua_gettop(L) == (__lua_stack_assert + (N)));
#define L_NOTNIL(L) assert(!lua_isnil(L, -1))

#endif // LUA_UTILS_H

