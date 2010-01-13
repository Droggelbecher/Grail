
#ifndef LUA_GAME_H
#define LUA_GAME_H

#include <map>

#include <lua.hpp>

#include "lua_utils.h"
#include "lib/game.h"

#include "lib/debug.h"
using std::endl;

class LuaGame : public Game {
    std::map<int, Chunk*> chapterConstructors;

  public:
    static LuaGame& getInstance();

    void registerChapterConstructor(int nr, Chunk* ctor) {
      if(chapterConstructors.count(nr)) {
        delete chapterConstructors[nr];
      }
      chapterConstructors[nr] = ctor;
    }

    void initChapter(size_t chapter);

};

#endif // LUA_GAME_H

