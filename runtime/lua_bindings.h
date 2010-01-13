
#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#include <iostream>
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include "lib/game.h"
#include "lib/debug.h"

class GameWrapper : public grail::Game {
  private:
    luabind::object initChapterCallback;

  public:


    static GameWrapper& getInstance() {
      using namespace grail;
      using namespace std;

      if(!_instance) {
        _instance = new GameWrapper();
      }
      // Just assume, _instance holds a GameWrapper, lets hope that stays
      // true!
      GameWrapper& r = *((GameWrapper*)_instance);
      return r;
    }

    virtual void initChapter(size_t n) {
      if(initChapterCallback) {
        luabind::call_function<size_t>(initChapterCallback, n);
      }
    }

    void setInitChapterCallback(luabind::object cb) {
      initChapterCallback = cb;
    }

};

extern "C" int init(lua_State* L);

#endif // LUA_BINDINGS_H

