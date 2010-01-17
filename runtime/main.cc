
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <getopt.h>

#include "interpreter.h"
#include "lib/viewport.h"
#include "lib/utils.h"
#include "wrappings.h"
#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lib/debug.h"

#include "lib/unittest.h"
#include "lua_bindings.h"

/*
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;
*/

Interpreter interpreter;

//void registerLuaWrappings();
//void registerHandcraftedWrappings();


int main(int argc, char** argv) {
  using namespace grail;
  using namespace std;
  using namespace luabind;

  GameWrapper& g = GameWrapper::getInstance();

  /*
  object gg = object(interpreter.L, g);
  gg.push(interpreter.L);
  lua_setglobal(interpreter.L, "GAME");
  */

  g.getResourceManager().mount(
      new DirectoryResourceHandler(argv[optind]), "/"
      );

  g.getResourceManager().mount(
      new DirectoryResourceHandler(dirName(argv[0]) + pathDelimiter + "prelude"), "/prelude"
      );

  init(interpreter.L);

  cdbg << "--- Prelude\n";
  interpreter.loadPrelude("/prelude");
  cdbg << "--- Game\n";
  interpreter.loadDirectory("/");

  delete &g;
}

