// vim: set noexpandtab:

#ifndef LUA_EXCEPTION_H
#define LUA_EXCEPTION_H

#include <exception>
#include <lua.hpp>

class LuaException : public std::exception {
	lua_State *L;
	
	public:
		LuaException(lua_State* L) throw() : L(L) {
		}
		
		const char* what() const throw() {
			return lua_tostring(L, -1);
		}
};

#endif // LUA_EXCEPTION_H

