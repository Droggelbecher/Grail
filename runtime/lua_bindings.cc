
#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <SDL/SDL.h>
#include <lua.hpp>

#include "lua_bindings.h"
#include "lib/animation.h"
#include "lib/game.h"
#include "lib/image.h"
#include "lib/resource_manager.h"
#include "lib/scene.h"
#include "lib/user_interface.h"
#include "lib/actor.h"
#include "lib/vector2d.h"
#include "lib/viewport.h"
#include "lib/sprite.h"
#include "lua_converter.h"
#include "lib/resource_manager.h"
#include "lua_chunk.h"

void test_luabind() {
  grail::cdbg << "If you can read this, luabind works\n";
}

void import(lua_State* L, std::string path) {
  grail::Resource resource(path, grail::MODE_READ);
  LuaChunk chunk(resource);
  chunk.load(L);
  lua_call(L, 0, 0);
}

extern "C" int init(lua_State* L) {
  using namespace luabind;
  using namespace grail;

  open(L);

  module(L)[
    def("test_luabind", &test_luabind),
    def("getGame", &GameWrapper::getInstance),
    def("import", &import, raw(_1)),

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
      .def("setInitChapterCallback", &GameWrapper::setInitChapterCallback)
      .def("setMainCharacter", &GameWrapper::setMainCharacter)
      .def("getMainCharacter", &GameWrapper::getMainCharacter)
      .def("runChapter", &GameWrapper::runChapter)
      ,

    class_<Actor, Actor::Ptr>("Actor")
      .def(constructor<std::string>())
      .def("hasPoint", &Actor::hasPoint)
      .def("addAnimation", &Actor::addAnimation)
      .def("setAlignment", &Actor::setAlignment)
      .def("setMode", &Actor::setMode)
      .def("setPosition", &Actor::setPosition)
      .def("getPosition", &Actor::getPosition)
      .def("walk", &Actor::walk)
      .def("walkStraight", &Actor::walkStraight)
      .def(tostring(self))
      ,

    class_<Animation, Animation::Ptr>("Animation"),

    class_<DirectionAnimation, Animation, Animation::Ptr>("DirectionAnimation")
      .def(constructor<uint16_t>())
      .def("setDirection", (void (DirectionAnimation::*)(uint16_t))&DirectionAnimation::setDirection)
      .def("setDirection", (void (DirectionAnimation::*)(VirtualPosition))&DirectionAnimation::setDirection)
      .def("setAnimation", &DirectionAnimation::setAnimation)
      ,

    class_<Image, Animation, Animation::Ptr>("Image")
      .def(constructor<std::string>())
      ,

    class_<ResourceManager>("ResourceManager")
      ,

    class_<Scene, Scene::Ptr>("Scene")
      .def(constructor<>())
      .def(constructor<Animation::ConstPtr>())
      .def(constructor<const std::string&>())
      .def("setBackground", &Scene::setBackground)
      .def("addActor", &Scene::addActor)
      .def("actorsMoved", &Scene::actorsMoved)
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

    class_<StripeSprite, Animation, Animation::Ptr>("StripeSprite")
      .def(constructor<std::string, size_t>())
      ,

    class_<UserInterface, UserInterfaceWrapper, UserInterface::Ptr>("UserInterface")
      .def(constructor<>())
      .def("handleEvent", &UserInterface::handleEvent, &UserInterfaceWrapper::default_handleEvent)
      ,

    class_<VirtualPosition>("VirtualPosition")
      .def(constructor<>())
      .def(constructor<VirtualPosition::X, VirtualPosition::Y>())
      .def("length", &VirtualPosition::length)
      .def("nearestDirection", &VirtualPosition::nearestDirection)
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
  ];

  return 0;
}

