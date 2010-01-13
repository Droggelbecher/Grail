
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>

#include "interpreter.h"
#include "lib/viewport.h"
#include "lib/utils.h"
#include "wrappings.h"
#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lua_user_interface.h"
#include "lua_game.h"

#include "lib/unittest.h"
#include "lua_bindings.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;

Interpreter interpreter;

void registerLuaWrappings();
void registerHandcraftedWrappings();


int main(int argc, char** argv) {
  Game& g = Game::getInstance();

  g.getResourceManager().mount(
      new DirectoryResourceHandler(argv[optind]), "/"
      );
  
  init(interpreter.L);
  interpreter.loadDirectory("/");

  delete &g;
}

