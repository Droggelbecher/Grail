
#ifndef LUA_USER_INTERFACE_H
#define LUA_USER_INTERFACE_H

#include "lib/user_interface.h"
#include "lua_utils.h"

#include <iostream>
using namespace std;

class LuaUserInterface : public UserInterface {
  public:
    EventState handleEvent(SDL_Event& event, uint32_t frameDuration) {
      L_STACK(interpreter.L);

      // method
      interpreter.pushWrapper(*this);
      lua_getfield(interpreter.L, -1, "handleEvent");
      lua_remove(interpreter.L, -2);

      if(lua_isnil(interpreter.L, -1)) {
        lua_pop(interpreter.L, 2);
        L_RETURN(interpreter.L, 0);
        return EVENT_STATE_UNHANDLED;
      }

      // this/self argument
      interpreter.pushWrapper(*this);

      // other arguments
      //interpreter.pushCopy<SDL_Event&>(event);
      luaPush<const SDL_Event&>(interpreter.L, event);
      lua_pushinteger(interpreter.L, frameDuration);

      lua_call(interpreter.L, 3, 1);

      int r = lua_tointeger(interpreter.L, -1);

      lua_pop(interpreter.L, 1);

      L_RETURN(interpreter.L, 0);
      return (EventState)r;
    }
};

#endif // LUA_USER_INTERFACE_H

