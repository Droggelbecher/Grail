
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


/**
 * _publish(clsname, ptr)
 * simply 'return' obj as clsname and ptr on the lua stack
 */
int publish(lua_State* L, Registrable& obj) {
  BEGIN_LUA_STACK_ASSERTION();

  lua_pushstring(L, obj.classname.c_str());
  lua_pushlightuserdata(L, (void*)(&obj));

  END_LUA_STACK_ASSERTION(2);

  return 2;
}

/**
 * Get named object from registry,
 * returns classname and ptr on lua stack
 */
int _get(lua_State* L) {
  string name = luaGet<string>(L, 1);
  Registrable& obj = Game::getInstance().getRegistry().get(name);
  publish(L, obj);
  return 2;
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
  }

  // name
  else if(classname == "Game") {
    obj = &(Game::getInstance());
    ((Game*)obj)->initChapter = &Game_initChapter;
  }

  // name
  else if(classname == "Scene") { obj = new Scene(); }
  else if(classname == "UserInterface") { obj = new UserInterface(); }
  else if(classname == "Actor") { obj = new Actor(); }

  else {
    throw Exception(string("Don't know how to construct a \"") + classname + string("\" object."));
  }

  assert(obj);
  obj->classname = classname;

  if(scope == 0) {
    Game::getInstance().getRegistry().registerApplication(*obj, name);
  }
  else {
    Game::getInstance().getRegistry().registerChapter(*obj, name);
  }

  publish(L, *obj);
  return 2;
}

/*
 * Debug functions
 */

int _debug_registryNames(lua_State* L) {
  Registry &reg = Game::getInstance().getRegistry();
  list<string> names = reg.getRegisteredNames();
  lua_newtable(L);
  int tbl = lua_gettop(L);
  list<string>::const_iterator iter;

  int i;
  for(i = 1, iter = names.begin(); iter != names.end(); iter++, i++) {
    lua_pushstring(L, iter->c_str());
    lua_rawseti(L, tbl, i);
  }
  return 1;
}



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

  lua_getglobal(L, "GAME");
  int t = lua_gettop(L);
  lua_getfield(L, t, "_chapter_ctors");

  lua_rawgeti(L, t+1, n); // function to call (chapter ctor for n)
  lua_pushinteger(L, n);
  int r = lua_pcall(L, 1, 0, 0);
  if(r)
    throw lua_exception(L);
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

void wrappings(Interpreter& i) {
  lua_State* L = interpreter.L;

  lua_register(L, "_debug_registryNames", &_debug_registryNames);

  lua_register(L, "_create", &_create);
  lua_register(L, "_get", &_get);

  lua_register(L, "_Game_instance", &_Game_instance);
  lua_register(L, "_Game_runChapter", &_Game_runChapter);
  lua_register(L, "_Game_getViewport", &_Game_getViewport);
  lua_register(L, "_Game_goToScene", &_Game_goToScene);
  lua_register(L, "_Game_getResourceManager", &_Game_getResourceManager);
  lua_register(L, "_Game_setUserInterface", &_Game_setUserInterface);
  lua_register(L, "_Game_getUserInterface", &_Game_getUserInterface);

  lua_register(L, "_Viewport_setup", &_Viewport_setup);

  lua_register(L, "_Scene_setBackground", &_Scene_setBackground);
  lua_register(L, "_Scene_addActor", &_Scene_addActor);

  lua_register(L, "_Actor_addAnimation", &_Actor_addAnimation);
  lua_register(L, "_Actor_setMode", &_Actor_setMode);
  lua_register(L, "_Actor_setPosition", &_Actor_setPosition);

}


