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
			ioService.poll_one();
			//network.select();
			Game::eachFrame(ticks);
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

class TaskWrapper : public grail::Task, public luabind::wrap_base {
	public:
		TaskWrapper(grail::Task::Flags flags = Task::DEFAULT) : grail::Task(flags) { }
		void eachFrame(uint32_t ticks) {
			call<void>("eachFrame", ticks);
		}
		static void default_eachFrame(Task::Ptr ptr, uint32_t ticks) {
			ptr->Task::eachFrame(ticks);
		}
		void onStart() {
			call<void>("onStart");
		}
		static void default_onStart(Task::Ptr ptr) {
			ptr->Task::onStart();
		}
}; // class TaskWrapper

class SceneWrapper : public grail::Scene, public luabind::wrap_base {
	public:
		SceneWrapper(grail::VirtualSize sz) : grail::Scene(sz) { }
		SceneWrapper(grail::Animation::Ptr anim) : grail::Scene(anim) { };
		SceneWrapper(const std::string& backgroundPath) : grail::Scene(backgroundPath) { };
		
		void onEnter() {
			call<void>("onEnter");
		}
		static void default_onEnter(grail::Scene::Ptr ptr) {
			ptr->Scene::onEnter();
		}
}; // clas SceneWrapper

extern "C" int init(lua_State* L);

#endif // LUA_BINDINGS_H

