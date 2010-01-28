
#ifndef LUA_CONVERTER_H
#define LUA_CONVERTER_H

#include <SDL/SDL_events.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "lib/event.h"
#include "lib/debug.h"

namespace luabind {
  template <>
  struct default_converter<SDL_Event> : native_converter_base<SDL_Event> {
    static int compute_score(lua_State* L, int index) {
      return 0;
      //return lua_type(L, index) == LUA_TTABLE ? 0 : -1;
    }

    SDL_Event from(lua_State* L, int index) {
      object o(from_stack(L, index));
      grail::Event e = object_cast<grail::Event>(o);
      return e.toSDL();
    }

    void to(lua_State* L, SDL_Event const& evt) {
      grail::Event e(evt);
      object o(L, e);
      o.push(L);
    }
  };
}

#endif // LUA_CONVERTER_H

