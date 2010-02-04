
#include "lua_game.h"
#include "interpreter.h"

LuaGame& LuaGame::getInstance() {
  if(!_instance) {
    _instance = new LuaGame();
  }
  return *(dynamic_cast<LuaGame*>(_instance));
}

void LuaGame::initChapter(size_t chapter) {
  cdbg << "Loading chapter constructor: " << chapterConstructors[chapter] << endl;
  chapterConstructors[chapter]->load(interpreter.L);
  luaPush<lua_Integer>(interpreter.L, chapter);
  lua_call(interpreter.L, 1, 0);
}


