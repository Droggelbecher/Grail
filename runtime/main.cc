
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>

#include "interpreter.h"
#include "viewport.h"
#include "utils.h"
#include "wrappings.h"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::list;

Interpreter interpreter;

int main(int argc, char** argv) {

  cout << "basedir:" << dirname(argv[0]) << endl;

  int c;
  while((c = getopt(argc, argv, "v")) != -1) {
    switch(c) {
      case 'v':
        cout << "verbose" << endl;
        break;
    }
  }

  if(optind < argc) {
    wrappings(interpreter);
    interpreter.loadPrelude(dirname(argv[0]) + path_delimiter + "prelude");
    interpreter.loadDirectory(argv[optind]);

    /*
    lua_getglobal(interpreter.L, "Game");
    lua_getfield(interpreter.L, -1, "_ptr");
    Game* g = (Game*)lua_touserdata(interpreter.L, -1);
    g->runChapter(0);
    */
  }
  else {
    cerr << "Please specify path to a game to run" << endl;
    return 1;
  }

  return 0;
}

