
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

/**
 * create(classname, scope, name, ...) -> (clsname, ptr)
 *
 * Object factory for lua
 */
int _create(lua_State* L) {
  size_t n = lua_gettop(L);

  string classname = luaGet<string>(L, 1);
  lua_Number scope = luaGet<lua_Number>(L, 2);

  string name = luaGet<string>(L, 3);

  Object* obj = 0;

  // --- Actual construction by class name ---
  // The comments describe arguments to this function after scope

  // path | path, name
  if(classname == "Image") {
    string path = name;
    if(n >= 3) {
      path = luaGet<string>(L, 4);
    }
    obj = new Image(path);
  }

  // name, path, frames
  else if(classname == "StripeSprite") {
    string path = luaGet<string>(L, 4);
    lua_Integer frames = luaGet<lua_Integer>(L, 5);
    cerr << "stripe sprite " << path << " " << frames << endl;
    obj = new StripeSprite(path, frames);
  }

  // name
  else if(classname == "Scene") { obj = new Scene(); }
  else if(classname == "UserInterface") { obj = new UserInterface(); }
  else if(classname == "Actor") { obj = new Actor(); }

  // name
  else {
    throw Exception(string("Don't know how to construct a \"") + classname + string("\" object."));
  }

  assert(obj);
  obj->className = classname;

  if(scope == 0) {
    Game::getInstance().getRegistry().registerApplication(*obj, name);
  }
  else {
    Game::getInstance().getRegistry().registerChapter(*obj, name);
  }

  return luaPush<Object&>(L, *obj);
}


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
  function("create", &_create);
  function("get", &_get);

  method("Game", "getInstance", &_Game_instance);
}

