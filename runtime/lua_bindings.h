// vim: set noexpandtab:

#ifndef LUA_BINDINGS_H
#define LUA_BINDINGS_H

#include <iostream>

#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <boost/shared_ptr.hpp>

#include "lib/game.h"
#include "lib/user_interface.h"
#include "lib/debug.h"
#include "network_interface.h"
#include "interpreter.h"
#include "to_lua_table.h"

class GameWrapper : public grail::Game {
	private:
		luabind::object initChapterCallback;
		NetworkInterface *server;
		boost::asio::io_service ioService;
		
		GameWrapper() : grail::Game() /*, network(12345)*/ {
			//network.bindLocal();
			//network.listen();
			server = new NetworkInterface(ioService);
			//ioService.run_one();
		}
		
	public:
		virtual ~GameWrapper() {
			delete server;
		}
		
		static GameWrapper& getInstance() {
			using namespace grail;
			using namespace std;
			
			if(!_instance) {
				_instance = new GameWrapper();
			}
			// Just assume, _instance holds a GameWrapper, lets hope that stays
			// true!
			GameWrapper& r = *((GameWrapper*)_instance);
			return r;
		}
		
		virtual void initChapter(size_t n) {
			if(initChapterCallback) {
				luabind::call_function<size_t>(initChapterCallback, n);
			}
		}
		
		void eachFrame(uint32_t ticks) {
			ioService.poll();
			//network.select();
			Game::eachFrame(ticks);
		}

		void handleEvent(const SDL_Event &event, uint32_t ticks) {
			grail::Event e(event);

			luabind::object o = toLuaTable(e);

			//luabind::object o(interpreter.L, e);
			server->broadcast(o);
		}
		
		void setInitChapterCallback(luabind::object cb) {
			initChapterCallback = cb;
		}
}; // class GameWrapper

class UserInterfaceWrapper : public grail::UserInterface, public luabind::wrap_base {
	public:
		UserInterfaceWrapper() { }
		grail::EventState handleEvent(const SDL_Event& event, uint32_t frameDuration) {
			return call<grail::EventState>("handleEvent", event, frameDuration);
		}
		
		static grail::EventState default_handleEvent(UserInterface::Ptr ptr, SDL_Event event, uint32_t frameDuration) {
			return ptr->UserInterface::handleEvent(event, frameDuration);
		}
}; // class UserInterfaceWrapper


extern "C" int init(lua_State* L);

#endif // LUA_BINDINGS_H

