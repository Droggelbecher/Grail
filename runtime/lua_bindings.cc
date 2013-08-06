// vim: set noexpandtab:

#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <SDL.h>
#include <lua.hpp>

#include "lib/action_text.h"
#include "lib/actor.h"
#include "lib/action.h"
#include "lib/animation.h"
#include "lib/audio.h"
#include "lib/box.h"
#include "lib/button.h"
#include "lib/font.h"
#include "lib/game.h"
#include "lib/ground.h"
#include "lib/image.h"
#include "lib/resource_manager.h"
#include "lib/scene.h"
#include "lib/sdl_exception.h"
#include "lib/sprite.h"
#include "lib/task.h"
#include "lib/wait_task.h"
#include "lib/text.h"
#include "lib/user_interface.h"
#include "lib/user_interface_animation.h"
#include "lib/user_interface_element.h"
#include "lib/vector2d.h"
#include "lib/viewport.h"
#include "lua_bindings.h"
#include "lua_button.h"
#include "lua_converter.h"

void test_luabind() {
	grail::cdbg << "If you can read this, luabind works\n";
}

extern "C" int init(lua_State* L) {
	using namespace luabind;
	using namespace grail;
	
	open(L);
	
	module(L)[
		def("test_luabind", &test_luabind),
		def("getGame", &GameWrapper::getInstance),
		def("rgb", (SDL_Color (*)(uint32_t))&rgb),
		def("rgb", (SDL_Color (*)(uint8_t, uint8_t, uint8_t))&rgb),
		
		class_<BlitCached>("_BlitCached")
		,
		
		class_<GameWrapper>("Game")
			.scope[
				def("getInstance", &GameWrapper::getInstance)
				]
			.def("getViewport", &GameWrapper::getViewport)
			.def("registerScene", &GameWrapper::registerScene)
			.def("getScene", &GameWrapper::getScene)
			.def("getCurrentScene", &GameWrapper::getCurrentScene)
			.def("goToScene", &GameWrapper::goToScene)
			.def("getResourceManager", &GameWrapper::getResourceManager)
			.def("getUserInterface", &GameWrapper::getUserInterface)
			.def("setUserInterface", &GameWrapper::setUserInterface)
			.def("initChapter", &GameWrapper::initChapter)
			.def("setEventCallback", &GameWrapper::setEventCallback)
			.def("setMainCharacter", &GameWrapper::setMainCharacter)
			.def("getMainCharacter", &GameWrapper::getMainCharacter)
			.def("runChapter", &GameWrapper::runChapter)
			.def("enableUserControl", &GameWrapper::enableUserControl)
			.def("quit", &GameWrapper::quit)
			,
		
		class_<Actor, Actor::Ptr>("Actor")
			.def(constructor<std::string>())
			.def("hasPoint", &Actor::hasPoint)
			.def("addAnimation", &Actor::addAnimation)
			.def("setAlignment", &Actor::setAlignment)
			.def("setMode", &Actor::setMode)
			.def("setPosition", &Actor::setPosition)
			.def("setYOffset", &Actor::setYOffset)
			.def("getPosition", &Actor::getPosition)
			.def("getName", &Actor::getName)
			.def("setSpeed", &Actor::setSpeed)
			.def("getSpeed", &Actor::getSpeed)
			.def("walk", &Actor::walk)
			.def("walkTo", (void(Actor::*)(VirtualPosition))&Actor::walkTo)
			.def("walkTo", (void(Actor::*)(Actor::Ptr))&Actor::walkTo)
			.def("walkStraight", &Actor::walkStraight)
			.def("say", (void(Actor::*)(std::string, uint32_t))&Actor::say)
			.def(tostring(self))
			,
		
		class_<Action, Action::Ptr>("Action")
			.def(constructor<std::string, size_t>())
			,
		
		class_<Animation, Animation::Ptr>("Animation"),
			
			class_<Box, Animation, Animation::Ptr>("Box")
				.def(constructor<VirtualSize>())
				.def(constructor<VirtualSize, SDL_Color>())
				,
			
			class_<DirectionAnimation, Animation, Animation::Ptr>("DirectionAnimation")
				.def(constructor<uint16_t>())
				.def(constructor<uint16_t, double>())
				.def("setDirection", (void (DirectionAnimation::*)(uint16_t))&DirectionAnimation::setDirection)
				.def("setDirection", (void (DirectionAnimation::*)(VirtualPosition))&DirectionAnimation::setDirection)
				.def("setAnimation", &DirectionAnimation::setAnimation)
				,
			
			class_<Image, Animation, Animation::Ptr>("Image")
				.def(constructor<std::string>())
				,
			
			class_<Text, Animation, BlitCached, Animation::Ptr>("Text")
				.def(constructor<Font::Ptr>())
				.def("getColor", &Text::getColor)
				.def("setColor", &Text::setColor)
				.def("getSize", &Text::getSize)
				.def("getOutlineColor", &Text::getOutlineColor)
				.def("setOutlineColor", &Text::setOutlineColor)
				.def("setOutlineFont", &Text::setOutlineFont)
				.def("setOutline", &Text::setOutline)
				.def("getText", &Text::getText)
				.def("setText", &Text::setText)
				,
			


		class_<Ground>("Ground")
			.scope[
				class_<Ground::Waypoint>("Waypoint")
					.def("getPosition", &Ground::Waypoint::getPosition)
					.def("linkBidirectional", &Ground::Waypoint::linkBidirectional)
				]
			
			.def("addPolygon", &Ground::addPolygon)
			//.def("generateMap", &Ground::generateMap)
			//.def("addWall", &Ground::addWall)
			//.def("addWalls", &Ground::addWalls)
			//.def("getWalls", &Ground::getWalls)
			//.def("addWaypoint", &Ground::addWaypoint)
			.def("directReachable", &Ground::directReachable)
			,
		
		class_<ResourceManager>("ResourceManager")
			.def("exists", &ResourceManager::exists)
			,
		
		class_<Scene, SceneWrapper, Scene::Ptr>("Scene")
			.def(constructor<Animation::Ptr>())
			.def(constructor<const std::string&>())
			.def(constructor<VirtualSize>())
			.def("setBackground", &Scene::setBackground)
			.def("addBackground", &Scene::addBackground)
			.def("addForeground", &Scene::addForeground)
			.def("addActor", &Scene::addActor)
			.def("actorsMoved", &Scene::actorsMoved)
			.def("getGround", &Scene::getGround)
			.def("enableDrawWalls", &Scene::enableDrawWalls)
			.def("onEnter", &Scene::onEnter, &SceneWrapper::default_onEnter)
			,
		
		class_<Event>("Event")
			.def(constructor<>())
			.def("getType", &Event::getType)
			.def("getButton", &Event::getButton)
			.def("getActor", &Event::getActor)
			.def("getPosition", &Event::getPosition)
			.def("getButtonState", &Event::getButtonState)
			//.def("getKeysym", &Event::getKeysym)
			,
		
		class_<Font, Font::Ptr>("Font")
			.def(constructor<std::string, int, int>())
			.def(constructor<const Font&>())
			,
		
		class_<StripeSprite, Animation, Animation::Ptr>("StripeSprite")
			.def(constructor<std::string, size_t>())
			.def(constructor<std::string, size_t, uint32_t>())
			,
		
		class_<ImageSprite, Animation, Animation::Ptr>("ImageSprite")
			.def(constructor<std::string>())
			.def(constructor<std::string, size_t>())
			,
		
		class_<VirtualPositionPolygon, VirtualPositionPolygon::Ptr>("VirtualPositionPolygon")
			.def(constructor<>())
			.def("hasPoint", &VirtualPositionPolygon::hasPoint)
			.def("push_back", &VirtualPositionPolygon::push_back)
			.def("clear", &VirtualPositionPolygon::clear)
			,
		
		class_<UserInterface, UserInterfaceWrapper, UserInterface::Ptr>("UserInterface")
			.def(constructor<>())
			.def("handleEvent", &UserInterface::handleEvent, &UserInterfaceWrapper::default_handleEvent)
			.def("addElement", &UserInterface::addElement)
			.def("addAnimation", &UserInterface::addAnimation)
			.def("setAction", &UserInterface::setAction)
			.def("getAction", &UserInterface::getAction)
			.def("getActionText", &UserInterface::getActionText)
			.def(tostring(self))
			,
		
		class_<UserInterfaceElement, UserInterfaceElement::Ptr>("UserInterfaceElement")
			.def("setPosition", &UserInterfaceElement::setPosition)
			.def("setAlignmentX", &UserInterfaceElement::setAlignmentX)
			.def("setAlignmentY", &UserInterfaceElement::setAlignmentY)
			,
		
			class_<ActionText, UserInterfaceElement, UserInterfaceElement::Ptr>("ActionText")
				.def(constructor<Font::Ptr>())
				.def("setOutlineFont", &ActionText::setOutlineFont)
				,
			
			class_<Button, UserInterfaceElement, UserInterfaceElement::Ptr>("_Button")
				,
			
			class_<LuaButton, Button, UserInterfaceElement::Ptr>("Button")
				.def(constructor<Animation::Ptr, luabind::object>())
				,
			
			class_<UserInterfaceAnimation, UserInterfaceElement, UserInterfaceElement::Ptr>("UserInterfaceAnimation")
				.def(constructor<Animation::Ptr>())
				,
		
		class_<VirtualPosition>("VirtualPosition")
			.def(constructor<>())
			.def(constructor<VirtualPosition::X, VirtualPosition::Y>())
			.def("length", &VirtualPosition::length)
			.def("nearestDirection", &VirtualPosition::nearestDirection)
			.def("cross", &VirtualPosition::cross)
			.def(-self)
			.def(double() * self)
			.def(self * double())
			.def(self + self)
			.def(self - self)
			.def(self == self)
			.def(tostring(self))
			.property("x", &VirtualPosition::getX, &VirtualPosition::setX)
			.property("y", &VirtualPosition::getY, &VirtualPosition::setY)
			,
		
		class_<Viewport>("Viewport")
			.def(constructor<>())
			.def(constructor<uint32_t, uint32_t>())
			.def(constructor<uint32_t, uint32_t, bool>())
			.def("setup", &Viewport::setup)
			.def("setFollowing", &Viewport::setFollowing)
			.def("setNoFollowing", &Viewport::setNoFollowing)
			,
		
		class_<Task, TaskWrapper, Task::Ptr>("Task")
			.def(constructor<>())
			.def(constructor<Task::Flags>())
			.def("start", &Task::start)
			.def("block", &Task::block)
			.def("eachFrame", &Task::eachFrame, &TaskWrapper::default_eachFrame)
			.def("onStart", &Task::onStart, &TaskWrapper::default_onStart)
			.def(tostring(self))
			,
		
		class_<WaitTask, Task, Task::Ptr>("WaitTask")
			.def(constructor<uint32_t>())
			,
		class_<SoundTask, Task, Task::Ptr>("SoundTask")
			.def(constructor<std::string, size_t>())
			.def("pause", &SoundTask::pause)
			,
		class_<Audio>("Audio")
			.def(constructor<>())
			.def("prepareSound", &Audio::prepareSound)
	];
	
	return 0;
}

