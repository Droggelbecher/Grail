
#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"

#define LUAGET(T) template T luaGet< T >(lua_State*, int);

LUAGET(Animation*);
LUAGET(Actor*);
LUAGET(Scene*);
LUAGET(Game*);
LUAGET(Viewport*);
LUAGET(UserInterface*);

