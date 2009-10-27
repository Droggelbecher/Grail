
#ifndef WRAPPINGS_H
#define WRAPPINGS_H

#include <cassert>
#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <lua.hpp>
#include "interpreter.h"

extern Interpreter interpreter;

// Viewport

int _Viewport_create(lua_State* L);
int _Viewport_setup(lua_State* L);

// Game

void Game_initChapter(size_t n);
int _Game_create(lua_State* L);
int _Game_runChapter(lua_State* L);
int _Game_getViewport(lua_State* L);
int _Game_goToScene(lua_State* L);
int _Game_getResourceManager(lua_State* L);

// Scene

int _Scene_create(lua_State* L);
int _Scene_setBackground(lua_State* L);

// Image

int _Image_create(lua_State* L);

void wrappings(Interpreter& i);

#endif // WRAPINGS_H

