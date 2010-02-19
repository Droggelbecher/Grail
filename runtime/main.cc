
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <getopt.h>

#include "interpreter.h"
#include "lib/debug.h"
#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lib/unittest.h"
#include "lib/utils.h"
#include "lib/viewport.h"
#include "lua_bindings.h"
#include "network_interface.h"
#include "wrappings.h"

Interpreter interpreter;
int main(int argc, char** argv) {
  using namespace grail;
  using namespace std;
  using namespace luabind;

  GameWrapper& g = GameWrapper::getInstance();

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

