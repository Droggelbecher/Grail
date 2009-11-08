
#include <map>

#include "wrappings.h"

#include "lib/game.h"
#include "lib/viewport.h"
#include "lib/scene.h"
#include "lib/animation.h"
#include "lib/registry.h"
#include "lib/image.h"
#include "lib/utils.h"
#include "lib/user_interface.h"

#include "lua_utils.h"

using std::string;
using std::map;


/**
 * _publish(clsname, ptr)
 * simply 'return' obj as clsname and ptr on the lua stack
 */
int publish(lua_State* L, Registrable& obj) {
  L_STACK(L);

  interpreter.pushWrapper(obj);

  L_RETURN(L, 1);
  return 1;
}

/**
 * Get named object from registry,
 * returns classname and ptr on lua stack
 */
int _get(lua_State* L) {
  string name = luaGet<string>(L, 1);
  Registrable& obj = Game::getInstance().getRegistry().get(name);
  return publish(L, obj);
}

/**
 */
int _wrap(lua_State* L) {
  assert(lua_isuserdata(L, 1));
  interpreter.pushWrapper(*((Registrable*)lua_touserdata(L, 1)));
  return 1;
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

  Registrable* obj = 0;

  // --- Actual construction by class name ---
  // The comments describe arguments to this function after scope

  // path | path, name
  if(classname == "Image") {
    string path = name;
    if(n >= 3) {
      path = luaGet<string>(L, 4);
    }
    obj = new Image(path);

    ((Image*)obj)->getSize();
    ((Animation*)obj)->getSize();
    ((Image*)(void*)obj)->getSize();
    ((Animation*)(void*)obj)->getSize();
  }

  // name
  else if(classname == "Scene") { obj = new Scene(); }
  else if(classname == "UserInterface") { obj = new UserInterface(); }
  else if(classname == "Actor") { obj = new Actor(); }

  // name
  /*
  else if(classname == "Game") {
    obj = &(Game::getInstance());
    ((Game*)obj)->initChapter = &Game_initChapter;
  }
  */

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

  return publish(L, *obj);
}

/*
 * Debug functions
 */
#ifdef DEBUG
int _debug_dumpRegistry(lua_State* L) {
  Registry &reg = Game::getInstance().getRegistry();
  map<string, Registry::RegistrableInfo> names = reg.registrables;
  lua_newtable(L);
  int tbl = lua_gettop(L);
  map<string, Registry::RegistrableInfo>::const_iterator iter;

  int i;
  for(i = 1, iter = names.begin(); iter != names.end(); iter++, i++) {
    lua_pushlightuserdata(L, iter->second.registrable);
    lua_setfield(L, tbl, iter->first.c_str());
  }
  return 1;
}
#endif // DEBUG


/*
 * Viewport
 */

int _Viewport_setup(lua_State* L) {
  assert(lua_gettop(L) >= 3);
  Viewport* vp = luaGet<Viewport*>(L, 1);
  lua_Integer x = luaGet<lua_Integer>(L, 2);
  lua_Integer y = luaGet<lua_Integer>(L, 3);
  bool fullscreen = false;
  if(lua_gettop(L) >= 4)
    fullscreen = luaGet<bool>(L, 4);

  vp->setup(x, y, fullscreen);
  return 0;
}

/*
 * Game
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
  return publish(L, *obj);
};


int _Game_runChapter(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  lua_Integer chapter = luaGet<lua_Integer>(L, 2);
  game->runChapter(chapter);
  return 0;
}

int _Game_getViewport(lua_State* L) {
  Game *game = luaGet<Game*>(L, 1);
  Viewport &vp = game->getViewport();
  assert(&vp);
  return publish(L, vp);
}

int _Game_goToScene(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  Scene* scene = luaGet<Scene*>(L, 2);
  game->goToScene(*scene);
  return 0;
}

int _Game_getResourceManager(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  ResourceManager &rm = game->getResourceManager();
  return publish(L, rm);
}

int _Game_setUserInterface(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  UserInterface* ui = luaGet<UserInterface*>(L, 2);
  game->setUserInterface(*ui);
  return 0;
}

int _Game_getUserInterface(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  return publish(L, (game->getUserInterface()));
}


/*
 * Scene
 */
int _Scene_setBackground(lua_State* L) {
  Scene* scene = luaGet<Scene*>(L, 1);
  const Animation& animation = (const Animation&)(*luaGet<Animation*>(L, 2));
  scene->setBackground(animation);
  return 0;
}

int _Scene_addActor(lua_State* L) {
  Scene* scene = luaGet<Scene*>(L, 1);
  Actor* actor = luaGet<Actor*>(L, 2);
  scene->addActor(*actor);
  return 0;
}

/*
 * Actor
 */
int _Actor_addAnimation(lua_State* L) {
  Actor* actor = luaGet<Actor*>(L, 1);
  string mode = luaGet<string>(L, 2);
  Animation* animation = luaGet<Animation*>(L, 3);
  animation->getSize();
  actor->addAnimation(mode, *animation);
  return 0;
}

int _Actor_setMode(lua_State* L) {
  Actor* actor = luaGet<Actor*>(L, 1);
  string mode = luaGet<string>(L, 2);
  actor->setMode(mode);
  return 0;
}

int _Actor_setPosition(lua_State* L) {
  Actor* actor = luaGet<Actor*>(L, 1);
  VirtualPosition p;
  p.setX(luaGet<lua_Integer>(L, 2));
  p.setY(luaGet<lua_Integer>(L, 3));
  actor->setPosition(p);
  return 0;
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

// #define METHOD(C, N) method(#C, #N, &_ ## C ## _ ## N )

void wrappings(Interpreter& i) {
  #ifdef DEBUG
  function("DBG_dumpRegistry", &_debug_dumpRegistry);
  #endif 

  function("create", &_create);
  function("get", &_get);
  function("wrap", &_wrap);

  method("Game", "instance", &_Game_instance);
  method("Game", "runChapter", &_Game_runChapter);
  method("Game", "getViewport", &_Game_getViewport);
  method("Game", "goToScene", &_Game_goToScene);
  method("Game", "getResourceManager", &_Game_getResourceManager);
  method("Game", "setUserInterface", &_Game_setUserInterface);
  method("Game", "getUserInterface", &_Game_getUserInterface);

  method("Viewport", "setup", &_Viewport_setup);

  method("Scene", "setBackground", &_Scene_setBackground);
  method("Scene", "addActor", &_Scene_addActor);

  method("Actor", "addAnimation", &_Actor_addAnimation);
  method("Actor", "setMode", &_Actor_setMode);
  method("Actor", "setPosition", &_Actor_setPosition);

  base("Image");

}


