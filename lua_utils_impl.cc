
#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"

//#define T(TYPE) template TYPE luaGet< TYPE >(lua_State* L, int n)

template Animation* luaGet< Animation* >(lua_State* L, int n);
/*
T(Animation*);
T(Scene*);
T(Game*);
T(Viewport*);
*/
