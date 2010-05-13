// vim: set noexpandtab:

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

#include <lua.hpp>
#include <luabind/luabind.hpp>
//#include <getopt.h>

#include "interpreter.h"
#include "lib/debug.h"
#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lib/unittest.h"
#include "lib/utils.h"
#include "lib/viewport.h"
#include "lua_bindings.h"
#include "network_interface.h"
#include "lib/version.h"

Interpreter interpreter;

void exitSyntax(char* self, int code) {
	using namespace std;
	cerr << endl << "Grail Adventure Game Engine v" VERSION << endl << endl
	    << "Usage: " << self << " [-h] [-v] [-p PRELUDEPATH] GAMEPATH" << endl << endl
			<< "  -p PRELUDEPATH   Load lua prelude from given path. Only use when you know what you are doing." << endl
			<< "  -v               Show version number and exit" << endl
			<< "  -h               Show this help and exit" << endl
			<< "  GAMEPATH         Path to directory of game to run. Try passing the 'demo' directory." << endl;

	exit(code);
}

int main(int argc, char** argv) {
	using namespace grail;
	using namespace std;
	using namespace luabind;
	
	string preludePath = dirName(argv[0]) + pathDelimiter + "prelude";

	int opt;
	while((opt = getopt(argc, argv, "hvp:")) != -1) {
		switch(opt) {
			case 'p':
				preludePath = string(optarg);
				break;

			case 'v':
				cout << "Grail Adventure Game Engine v" VERSION << endl;
				exit(0);
				break;

			case 'h':
				exitSyntax(argv[0], 0);
				break;
				
			default:
				exitSyntax(argv[0], 1);
				break;
		}
	}

	if(optind >= argc) {
		exitSyntax(argv[0], 2);
	}
	
	GameWrapper& g = GameWrapper::getInstance();
	
	g.getResourceManager().mount(
		new DirectoryResourceHandler(argv[optind]), "/"
		);
	
	g.getResourceManager().mount(
		new DirectoryResourceHandler(preludePath), "/prelude"
		);
	
	init(interpreter.L);
	
	cdbg << "--- Prelude\n";
	interpreter.loadPrelude("/prelude");
	cdbg << "--- Game\n";
	interpreter.loadDirectory("/");
	
	delete &g;
}

