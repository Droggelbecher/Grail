
#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"
#include "lib/user_interface.h"

#define LUAGET(T) template T luaGet< T >(lua_State*, int);

LUAGET(Actor*);
LUAGET(Animation*);
LUAGET(Game*);
LUAGET(Registrable*);
LUAGET(Scene*);
LUAGET(UserInterface*);
LUAGET(Viewport*);

