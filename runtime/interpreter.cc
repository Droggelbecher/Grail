// vim: set noexpandtab:

#include <boost/regex.hpp>
#include <iostream>

#include <lua.hpp>

#include "interpreter.h"
#include "lib/utils.h"
#include "lib/shortcuts.h"
#include "lib/vector2d.h"
#include "lib/resource_manager.h"
#include "lib/event.h"
#include "lib/debug.h"
#include "lua_exception.h"

using std::string;
using std::cerr;
using std::endl;
using namespace grail;

void Interpreter::getOrCreateAsEmptyTable(std::string fieldName) {
	lua_getfield(L, -1, fieldName.c_str());
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_newtable(L);
		lua_pushvalue(L, -1);
		lua_setfield(L, -3, fieldName.c_str());
	}
}

void Interpreter::makeBase() {
	lua_getfield(L, -1, "__index");
	if(lua_isnil(L, -1)) {
		lua_pop(L, 1);
		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");
	}
	else {
		lua_pop(L, 1);
	}
}

int Interpreter::l_panic(lua_State* L) {
	throw LuaException(L);
	return 0;
}

Interpreter::Interpreter() {
	L = luaL_newstate();
	lua_atpanic(L, &l_panic);
	luaL_openlibs(L);

	lua_register(L, "doresource", &l_doresource);
}

Interpreter::~Interpreter() {
	lua_close(L);
}

int Interpreter::l_doresource(lua_State* L) {
	const char* p = lua_tostring(L, 1);
	interpreter.runLuaFromResource(string(p));
	return 0;
}

void Interpreter::runLuaFromResource(string path) {
	Resource r(path, MODE_READ);
	
	luaL_loadbuffer(L, r.getData(), r.getDataSize(), r.path.c_str());
	int error = lua_pcall(L, 0, LUA_MULTRET, 0);
	
	if(error) {
		throw LuaException(L);
	}
}

void Interpreter::loadDirectory(string dir) {
	runLuaFromResource(dir + "/init.lua");
}

void Interpreter::loadPrelude(string dir) {
	runLuaFromResource(dir + "/prelude.lua");
}

std::string Interpreter::toLuaString(luabind::object obj, std::string indent) {
	std::string r;
	
	switch(luabind::type(obj)) {
		case LUA_TTABLE:
			r = "{\n";
			for(luabind::iterator iter(obj), end; iter != end; ++iter) {
				r += indent + "  [" + toLuaString(iter.key()) + "] = " + toLuaString(*iter, indent + "	") + ",\n";
			}
			r += indent + "}";
			break;
			
		case LUA_TNIL:
			r = "nil";
			break;
			
		case LUA_TBOOLEAN:
			if(obj) { r = "true"; }
			else { r = "false"; }
			break;
			
		case LUA_TNUMBER:
			r = toString(luabind::object_cast<double>(obj));
			break;
			
		case LUA_TSTRING:
			r = toLuaString(luabind::object_cast<std::string>(obj));
			break;
			
		default:
			r = "__something__";
			//throw Exception("Couldn't represent lua thingy of type " + toString(luabind::type(obj)) + ".");
			break;
	}
	return r;
} // toLuaString

std::string Interpreter::toLuaString(std::string s) {
	size_t p = static_cast<size_t>(-1);
	
	// " -> \"
	const boost::regex quote("(^|[^\\\\])(\\\\\\\\)*(\")");
	const std::string quote_replacement("\\1\\2\\\\\\3");
	s = boost::regex_replace(s, quote, quote_replacement, boost::match_default | boost::format_sed);
	
	const boost::regex newline("\\n");
	const std::string newline_replacement("\\\\n");
	s = boost::regex_replace(s, newline, newline_replacement, boost::match_default | boost::format_sed);
	
	// \ -> \\	. 
	while((p = s.find('\'', p+1)) != std::string::npos) {
		s = s.substr(0, p) + s.substr(p, s.size());
	}
	return "\"" + s + "\"";
}



