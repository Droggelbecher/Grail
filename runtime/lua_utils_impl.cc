
#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"
#include "lib/user_interface.h"
#include "lib/ground.h"

#define LUAGET(T) template T luaGet< T >(lua_State*, int);
#define LUAPUSH(T) template int luaPush< T >(lua_State*, T);

LUAGET(lua_Number);
LUAGET(lua_Integer);
LUAGET(bool);
LUAGET(std::string);
LUAGET(VirtualPosition);

LUAGET(Actor&);
LUAGET(Actor*);
LUAGET(Animation&);
LUAGET(Animation*);
LUAGET(DirectionAnimation&);
LUAGET(DirectionAnimation*);
LUAGET(Game&);
LUAGET(Game*);
LUAGET(Ground&);
LUAGET(Ground*);
LUAGET(Object&);
LUAGET(Object*);
LUAGET(Polygon&);
LUAGET(Polygon*);
LUAGET(Scene&);
LUAGET(Scene*);
LUAGET(UserInterface&);
LUAGET(UserInterface*);
LUAGET(Viewport&);
LUAGET(Viewport*);

LUAPUSH(const SDL_Event&);
LUAPUSH(lua_Integer);

LUAPUSH(Actor&);
LUAPUSH(Animation&);
LUAPUSH(Game&);
LUAPUSH(Object&);
LUAPUSH(Scene&);
LUAPUSH(UserInterface&);
LUAPUSH(Viewport&);
LUAPUSH(ResourceManager&);
LUAPUSH(Ground&);

