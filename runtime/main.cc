
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>
#include <getopt.h>

#include "interpreter.h"
#include "lib/viewport.h"
#include "lib/utils.h"
#include "wrappings.h"
#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lua_user_interface.h"

#include "lib/unittest.h"

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

  Unittest::runAll();

  cout << "basedir:" << dirName(argv[0]) << endl;

  int c;
  while((c = getopt(argc, argv, "v")) != -1) {
    switch(c) {
      case 'v':
        cout << "verbose" << endl;
        break;
    }
  }

  if(optind < argc) {
    Game& g = Game::getInstance();

    g.getResourceManager().mount(
        new DirectoryResourceHandler(argv[optind]), "/"
        );

    g.getResourceManager().mount(
        new DirectoryResourceHandler(dirName(argv[0]) + pathDelimiter + "prelude"),
        "/prelude"
        );

    g.setUserInterface(*(new LuaUserInterface()));

    registerLuaWrappings();
    registerHandcraftedWrappings();

    interpreter.loadPrelude("/prelude");
    interpreter.loadDirectory("/");

    delete &g;
  }
  else {
    cerr << "Please specify path to a game to run" << endl;
    return 1;
  }

  return 0;
}

