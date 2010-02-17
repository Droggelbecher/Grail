
#ifndef LUA_BUTTON_H
#define LUA_BUTTON_H

#include "lib/button.h"
#include "lib/animation.h"
#include <luabind/luabind.hpp>
#include <SDL/SDL.h>

namespace grail {

  class LuaButton : public Button {
      luabind::object luaCallback;

    public:
      LuaButton(Animation::Ptr animation, luabind::object luaCallback) :
        Button(animation, &LuaButton::callbackWrapper), luaCallback(luaCallback) {
      }

      static EventState callbackWrapper(Button& button , const SDL_Event& event, uint32_t ticks) {
        LuaButton& bt = dynamic_cast<LuaButton&>(button);
        return luabind::call_function<EventState>(bt.luaCallback, button, event, ticks);
      }
  };

} // namespace

#endif // LUA_BUTTON_H



