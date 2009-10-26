
#ifndef WRAPPINGS_H
#define WRAPPINGS_H

#include <cassert>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <lua.hpp>

#include "lib/game.h"
#include "lib/viewport.h"
#include "lib/scene.h"
#include "lib/image.h"

extern Interpreter interpreter;

/**
 * luaGet<Type, int N>(lua_State* L)
 *
 * Get data from stack at position N as type Type
 */
template <typename T>
T luaGet(lua_State* L, int N) {
  assert(lua_istable(L, N));
  lua_getfield(L, N, "_ptr");

  int top = lua_gettop(L);
  assert(lua_isuserdata(L, top));
  T r = (T)lua_touserdata(L, top);
  lua_pop(L, 1);
  return r;
};

template <> lua_Number luaGet<lua_Number>(lua_State* L, int N) { return lua_tonumber(L, N); };
template <> lua_Integer luaGet<lua_Integer>(lua_State* L, int N) { return lua_tointeger(L, N); };
template <> bool luaGet<bool>(lua_State* L, int N) { return lua_toboolean(L, N); };
template <> std::string luaGet<std::string>(lua_State* L, int N) {
  size_t len;
  const char* s = lua_tolstring(L, N, &len);
  return std::string(s, len);
}

/**
 * Pushes a wrapper table for the given pointer
 */
void luaPushWrapper(lua_State* L, void* data, const char* className) {
  lua_getglobal(L, "_wrap_or_get");
  lua_pushlightuserdata(L, data);
  lua_pushstring(L, className);
  lua_call(L, 2, 1);
}

/*
 * Viewport
 */

int _Viewport_create(lua_State* L) {
  Viewport* vp = new Viewport();
  luaPushWrapper(L, (void*)vp, "viewport");
  return 1;
}

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

  lua_getglobal(L, "Game");
  int t = lua_gettop(L);
  lua_getfield(L, t, "_chapter_ctors");

  lua_rawgeti(L, t+1, n); // function to call (chapter ctor for n)
  lua_pushinteger(L, n);
  int r = lua_pcall(L, 1, 0, 0);
  if(r)
    throw lua_exception(L);
}

int _Game_create(lua_State* L) {
  void *obj = (void*)(new Game());
  ((Game*)obj)->initChapter = &Game_initChapter;
  assert(obj != NULL);
  //lua_pushlightuserdata(L, obj);
  luaPushWrapper(L, obj, "game");
  return 1;
};


int _Game_runChapter(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  lua_Integer chapter = luaGet<lua_Integer>(L, 2);
  game->runChapter(chapter);
  return 0;
}

int _Game_getViewport(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  luaPushWrapper(L, (void*)&(game->getViewport()), "viewport");
  //lua_pushlightuserdata(L, (void*)&(game->getViewport()));
  return 1;
}

int _Game_goToScene(lua_State* L) {
  Game* game = luaGet<Game*>(L, 1);
  Scene* scene = luaGet<Scene*>(L, 2);
  game->goToScene(*scene);
  return 0;
}

/*
 * Scene
 */
int _Scene_create(lua_State* L) {
  Scene* scene = new Scene();
  luaPushWrapper(L, (void*)scene, "scene");
  return 1;
}

int _Scene_setBackground(lua_State* L) {
  Scene* scene = luaGet<Scene*>(L, 1);
  const Image& image = (const Image&)(*luaGet<Image*>(L, 2));
  scene->setBackground(image);
  return 0;
}

/*
 * Image
 */

int _Image_create(lua_State* L) {
  std::string path = luaGet<std::string>(L, 1);
  Image* image = new Image(path);
  luaPushWrapper(L, (void*)image, "image");
  //lua_pushlightuserdata(L, (void*)image);
  return 1;
}


void wrappings(Interpreter& i) {
  lua_State* L = interpreter.L;

  lua_register(L, "_Game_create", &_Game_create);
  lua_register(L, "_Game_runChapter", &_Game_runChapter);
  lua_register(L, "_Game_getViewport", &_Game_getViewport);
  lua_register(L, "_Game_goToScene", &_Game_goToScene);

  lua_register(L, "_Viewport_create", &_Viewport_create);
  lua_register(L, "_Viewport_setup", &_Viewport_setup);

  lua_register(L, "_Scene_create", &_Scene_create);
  lua_register(L, "_Scene_setBackground", &_Scene_setBackground);

  lua_register(L, "_Image_create", &_Image_create);
}


#endif // WRAPINGS_H

