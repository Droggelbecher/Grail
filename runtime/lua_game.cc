
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
  //cdbg << "t=" << lua_typename(interpreter.L, lua_type(interpreter.L, -2)) << endl;
  lua_call(interpreter.L, 1, 0);
}


