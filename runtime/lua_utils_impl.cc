
#include <string>

#include "lua_utils.cc"

#include "lib/animation.h"
#include "lib/game.h"
#include "lib/scene.h"
#include "lib/viewport.h"
#include "lib/user_interface.h"
#include "lib/ground.h"
#include "lib/sprite.h"
#include "runtime/lua_user_interface.h"
#include "runtime/lua_game.h"

#define LUAGET(T) template T luaGet< T >(lua_State*, int);
#define LUAPUSH(T) template int luaPush< T >(lua_State*, T);
#define PUSHREF(T) template int luaPushReference< T >(lua_State*, T*, std::string);
#define PUSH(T) LUAPUSH(T*); LUAPUSH(T&); PUSHREF(T)

LUAGET(lua_Number);
LUAGET(lua_Integer);
LUAGET(bool);
LUAGET(std::string);
LUAGET(VirtualPosition);

LUAGET(Actor&);
LUAGET(Actor*);
LUAGET(Animation&);
LUAGET(Animation*);
LUAGET(Chunk*);
LUAGET(DirectionAnimation&);
LUAGET(DirectionAnimation*);
LUAGET(Game&);
LUAGET(Game*);
//LUAGET(LuaGame&);
//LUAGET(LuaGame*);
LUAGET(Ground&);
LUAGET(Ground*);
LUAGET(Polygon&);
LUAGET(Polygon*);
LUAGET(Scene&);
LUAGET(Scene*);
LUAGET(SDL_Event&);
LUAGET(UserInterface&);
LUAGET(UserInterface*);
//LUAGET(LuaUserInterface&);
//LUAGET(LuaUserInterface*);
LUAGET(Viewport&);
LUAGET(Viewport*);

LUAPUSH(const SDL_Event&);
LUAPUSH(lua_Integer);

PUSH(Actor);
PUSH(Animation);
PUSH(DirectionAnimation);
PUSH(Game);
//PUSH(LuaGame);
PUSH(Ground);
PUSH(Image);
//PUSH(LuaUserInterface);
PUSH(ResourceManager);
PUSH(Scene);
PUSH(StripeSprite);
PUSH(UserInterface);
PUSH(Viewport);

