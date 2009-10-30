
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>

#include "interpreter.h"
#include "viewport.h"
#include "utils.h"
#include "wrappings.h"
#include "lib/game.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;

Interpreter interpreter;

int main(int argc, char** argv) {

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
    Game::getInstance().getResourceManager().mount(
        new DirectoryResourceHandler(argv[optind]),
        "/"
        );

    wrappings(interpreter);
    interpreter.loadPrelude(dirName(argv[0]) + pathDelimiter + "prelude");
    interpreter.loadDirectory("/");
  }
  else {
    cerr << "Please specify path to a game to run" << endl;
    return 1;
  }

  return 0;
}

