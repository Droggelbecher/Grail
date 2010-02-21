
#ifndef LUA_CONVERTER_H
#define LUA_CONVERTER_H

#include <SDL/SDL_events.h>
#include <lua.hpp>
#include <luabind/luabind.hpp>

#include "lib/event.h"
#include "lib/debug.h"
#include "lib/sdlutils.h"
#include "lib/debug.h"
#include "lib/vector2d.h"

namespace luabind {
  template <>
  struct default_converter<SDL_Event> : native_converter_base<SDL_Event> {
    static int compute_score(lua_State* L, int index) {
      //return 0;
      //return lua_type(L, index) == LUA_TTABLE ? 0 : -1;
      return lua_type(L, index) == LUA_TUSERDATA ? 0 : -1;
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

  template <>
  struct default_converter<SDL_Color> : native_converter_base<SDL_Color> {
    static int compute_score(lua_State* L, int index) {
      return lua_type(L, index) == LUA_TNUMBER ? 0 : -1;
    }

    SDL_Color from(lua_State* L, int index) {
      object o(from_stack(L, index));
      uint32_t c = object_cast<uint32_t>(o);
      return grail::rgb(c);
    }

    void to(lua_State* L, SDL_Color const& color) {
      object o(L, (color.r << 16) | (color.g << 8) | color.b);
      o.push(L);
    }
  };

  template <>
  struct default_converter<grail::VirtualPosition> : native_converter_base<grail::VirtualPosition> {
    static int compute_score(lua_State* L, int index) {
      return (lua_type(L, index) == LUA_TTABLE) /*|| (lua_type(L, index) == LUA_TUSERDATA)*/ ? 0 : -1;
    }

    grail::VirtualPosition from(lua_State* L, int index) {
      grail::VirtualPosition r;
      object o(from_stack(L, index));

      //if(lua_type(L, index) == LUA_TTABLE) {
        iterator i(o);
        r.setX(object_cast<grail::VirtualPosition::X>(*i));
        ++i;
        r.setY(object_cast<grail::VirtualPosition::Y>(*i));
      //}
      /*else if(lua_type(L, index) == LUA_TUSERDATA) {
        r = reinterpret_cast<grail::VirtualPosition>(touserdata<grail::VirtualPosition>(o));
        grail::cdbg << "x=" << r.getX() << " y=" << r.getY() << "\n";
      }*/

      return r;
    }

    void to(lua_State* L, grail::VirtualPosition p) {
      object o = newtable(L);
      o[1] = p.getX();
      o[2] = p.getY();
      o.push(L);
    }
  };

}

#endif // LUA_CONVERTER_H

