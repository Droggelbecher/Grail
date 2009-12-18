
#include <cassert>
#include <string>
#include <sstream>

#include "lib/actor.h"
#include "lib/animation.h"
#include "lib/area.h"
#include "lib/classes.h"
#include "lib/event.h"
#include "lib/game.h"
#include "lib/ground.h"
#include "lib/image.h"
#include "lib/mainloop.h"
#include "lib/rect.h"
#include "lib/registry.h"
#include "lib/resource_manager.h"
#include "lib/scene.h"
#include "lib/shortcuts.h"
#include "lib/sprite.h"
#include "lib/surface.h"
#include "lib/unittest.h"
#include "lib/user_interface.h"
#include "lib/user_interface_element.h"
#include "lib/utils.h"
#include "lib/vector2d.h"
#include "lib/viewport.h"

#include "lua_utils.h"
#include "interpreter.h"

using std::istringstream;
using std::string;

/**
 * Get named object from registry,
 * returns classname and ptr on lua stack
 */
int _get(lua_State* L) {
  string name = luaGet<string>(L, 1);
  Object& obj = Game::getInstance().getRegistry().get(name);
  return luaPush<Object&>(L, obj);
}

int registerChapter(lua_State* L) {
  Object* obj = luaGet<Object*>(L, 1);
  string name = luaGet<string>(L, 2);

  Game::getInstance().getRegistry().registerChapter(*obj, name);
  return 0;
}

int registerApplication(lua_State* L) {
  Object* obj = luaGet<Object*>(L, 1);
  string name = luaGet<string>(L, 2);

  Game::getInstance().getRegistry().registerApplication(*obj, name);
  return 0;
}

int _pushSceneElement(lua_State* L, map<string, string>& parameters) {
  if(parameters["type"] == "Ground") {
    Ground* g = new Ground;
    SplitIterator nodeIter = SplitIterator(parameters["nodes"], ";"),
                  edgeIter = SplitIterator(parameters["edges"], ";");

    vector<Ground::Waypoint*> nodes;
    for(; nodeIter != nodeIter.end(); nodeIter++) {
      pair<string, string> xy = split2(*nodeIter);
      Ground::Waypoint *wp = new Ground::Waypoint(
          VirtualPosition(fromString<VirtualPosition::X>(xy.first), fromString<VirtualPosition::Y>(xy.second))
      );
      nodes.push_back(wp);
      g->addWaypoint(*wp);
    }

    for(; edgeIter != edgeIter.end(); edgeIter++) {
      pair<string, string> ab = split2(*edgeIter);
      size_t a = fromString<size_t>(ab.first);
      size_t b = fromString<size_t>(ab.second);
      assert(a < nodes.size());
      assert(b < nodes.size());
      nodes[a]->linkBidirectional(*nodes[b]);
    }

    return luaPush<Ground&>(L, *g);
  }

  return 0;
}

int loadSceneDefinition(lua_State* L) {
  string res = luaGet<string>(L, 1);

  Resource f(res, MODE_READ);
  istringstream s(string(f.getData(), f.getDataSize()));
  char linebuffer[2048]; // lines can be quite long
  map<string, string> parameters;
  bool ignore_block = false;

  lua_newtable(L);

  while(!s.eof()) {
    s.getline(linebuffer, 2048-1);
    string line = string(linebuffer);
    if(line.length() == 0) continue; // ignore empty lines

    if(startsWith(line, "[element ")) {
      if(_pushSceneElement(L, parameters)) {
        lua_setfield(L, -2, parameters["name"].c_str());
      }

      parameters.clear();
      ignore_block = false;
      parameters["name"] = line.substr(9, line.length() - 10);
    }
    else if(line == "[scene]") {
      // ignore [scene] blocks
      ignore_block = true;
    }
    else if(line == "[scene background]") {
    }
    else if(!ignore_block) {
      pair<string, string> kv = split2(line, "=");
      parameters[strip(kv.first)] = strip(kv.second);
    }
  }

  if(_pushSceneElement(L, parameters)) {
    lua_setfield(L, -2, parameters["name"].c_str());
  }

  return 1;
} // loadSceneDefinition

void Game_initChapter(size_t n) {
  lua_State* L = interpreter.L;

  interpreter.pushWrapper(Game::getInstance());
  size_t t = lua_gettop(L);

  lua_getfield(L, t, "_chapter_ctors");
  lua_rawgeti(L, t+1, n); // function to call (chapter ctor for n)
  lua_pushinteger(L, n);
  int r = lua_pcall(L, 1, 0, 0);
  if(r)
    throw LuaException(L);
}

int _Game_instance(lua_State* L) {
  Game *obj = &(Game::getInstance());
  obj->initChapter = &Game_initChapter;
  assert(obj != NULL);
  return luaPush<Game&>(L, *obj);
}



inline void function(std::string name, lua_CFunction fn) {
  interpreter.registerFunction(name, fn);
}
inline void method(std::string baseName, std::string name, lua_CFunction fn) {
  interpreter.registerMethod(baseName, name, fn);
}
inline void base(std::string baseName) {
  interpreter.registerBase(baseName);
}

void registerHandcraftedWrappings() {
  function("get", &_get);

  function("registerChapter", &registerChapter);
  function("registerApplication", &registerApplication);
  function("loadSceneDefinition", &loadSceneDefinition);

  method("Game", "getInstance", &_Game_instance);
}

