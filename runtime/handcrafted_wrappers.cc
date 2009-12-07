
#include <cassert>
#include <string>

#include "lib/actor.h"
#include "lib/animation.h"
#include "lib/area.h"
#include "lib/classes.h"
#include "lib/event.h"
#include "lib/game.h"
#include "lib/image.h"
#include "lib/mainloop.h"
#include "lib/rect.h"
#include "lib/registry.h"
#include "lib/resource_manager.h"
#include "lib/scene.h"
#include "lib/shortcuts.h"
#include "lib/sprite.h"
#include "lib/surface.h"
#include "lib/unittest.h"
#include "lib/user_interface_element.h"
#include "lib/user_interface.h"
#include "lib/utils.h"
#include "lib/vector2d.h"
#include "lib/viewport.h"

#include "lua_utils.h"
#include "interpreter.h"

/**
 * Get named object from registry,
 * returns classname and ptr on lua stack
 */
int _get(lua_State* L) {
  string name = luaGet<string>(L, 1);
  Object& obj = Game::getInstance().getRegistry().get(name);
  return luaPush<Object&>(L, obj);
}

int registerChapter(lua_State* L) {
  Object* obj = luaGet<Object*>(L, 1);
  string name = luaGet<string>(L, 2);

  Game::getInstance().getRegistry().registerChapter(*obj, name);
  return 0;
}

int registerApplication(lua_State* L) {
  Object* obj = luaGet<Object*>(L, 1);
  string name = luaGet<string>(L, 2);

  Game::getInstance().getRegistry().registerApplication(*obj, name);
  return 0;
}

// XXX

/*
void _pushSceneElement(map<string, string> parameters) {
  if(parameters["type"] == "Ground") {
    Ground* g = new Ground;
    g.nodes = [VirtualPosition(x) for x in parametrs["nodes"].split(";")]
  }
  else if( ... ) {
    ...
  }
}

int loadSceneDefinition(lua_State* L) {
  string res = luaGet<string>(L, 1);
  Resource f(res, MODE_READ);
  // TODO: Read from char* line by line

  for(...) {
    line = line.strip();
    if(line.size() && line[0] == '[') {
      if [element ...] {
        ... -> name
      }
      else if [scene] {
        ignore
      }
      else if [scene background] {
        "background" -> name
      }
    }
    else {
      if type = ... {
        ... -> type
      }
  }
} // loadSceneDefinition
*/

void Game_initChapter(size_t n) {
  lua_State* L = interpreter.L;

  interpreter.pushWrapper(Game::getInstance());
  size_t t = lua_gettop(L);

  lua_getfield(L, t, "_chapter_ctors");
  lua_rawgeti(L, t+1, n); // function to call (chapter ctor for n)
  lua_pushinteger(L, n);
  int r = lua_pcall(L, 1, 0, 0);
  if(r)
    throw LuaException(L);
}

int _Game_instance(lua_State* L) {
  Game *obj = &(Game::getInstance());
  obj->initChapter = &Game_initChapter;
  assert(obj != NULL);
  return luaPush<Game&>(L, *obj);
}



inline void function(std::string name, lua_CFunction fn) {
  interpreter.registerFunction(name, fn);
}
inline void method(std::string baseName, std::string name, lua_CFunction fn) {
  interpreter.registerMethod(baseName, name, fn);
}
inline void base(std::string baseName) {
  interpreter.registerBase(baseName);
}

void registerHandcraftedWrappings() {
  function("get", &_get);

  function("registerChapter", &registerChapter);
  function("registerApplication", &registerApplication);
  //function("loadSceneDefinition", &loadSceneDefinition);

  method("Game", "getInstance", &_Game_instance);
}

