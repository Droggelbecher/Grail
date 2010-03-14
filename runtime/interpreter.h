// vim: set noexpandtab:

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <string>
#include <lua.hpp>
#include <luabind/luabind.hpp>

class Interpreter {
		static int l_panic(lua_State* L);
		static int l_doresource(lua_State* L);
		
		void pushInternalTable();
		void getOrCreateAsEmptyTable(std::string fieldName);
		void makeBase();
		
	public:
		lua_State *L;
		
		Interpreter();
		~Interpreter();
		
		void runLuaFromResource(std::string path);
		void loadDirectory(std::string dir);
		void loadPrelude(std::string dir);
		std::string toLuaString(luabind::object obj, std::string indent="");
		std::string toLuaString(std::string s);
};

extern Interpreter interpreter;

#endif // INTERPRETER_H

