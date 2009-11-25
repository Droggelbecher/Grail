
#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"
#include "lib/user_interface.h"

#define LUAGET(T) template T luaGet< T >(lua_State*, int);
#define LUAPUSH(T) template int luaPush< T >(lua_State*, T);

LUAGET(lua_Number);
LUAGET(lua_Integer);
LUAGET(bool);
LUAGET(std::string);
LUAGET(VirtualPosition);

LUAGET(Actor*);
LUAGET(Animation*);
LUAGET(DirectionAnimation*);
LUAGET(Game*);
LUAGET(Scene*);
LUAGET(UserInterface*);
LUAGET(Viewport*);
LUAGET(Object*);

LUAGET(Actor&);
LUAGET(Animation&);
LUAGET(DirectionAnimation&);
LUAGET(Game&);
LUAGET(Scene&);
LUAGET(UserInterface&);
LUAGET(Viewport&);
LUAGET(Object&);

LUAPUSH(const SDL_Event&);

LUAPUSH(Actor&);
LUAPUSH(Animation&);
LUAPUSH(Game&);
LUAPUSH(Object&);
LUAPUSH(Scene&);
LUAPUSH(UserInterface&);
LUAPUSH(Viewport&);
LUAPUSH(ResourceManager&);

