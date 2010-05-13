// vim: set noexpandtab ts=4 sw=4:


#ifndef TO_LUA_TABLE_H
#define TO_LUA_TABLE_H

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include "lib/event.h"

luabind::object toLuaTable(const grail::Event&);

#endif // TO_LUA_TABLE_H

