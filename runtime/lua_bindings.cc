
#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <SDL/SDL.h>

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
      .def("runChapter", &GameWrapper::runChapter)
      ,

    class_<Actor, Actor::Ptr>("Actor")
      .def(constructor<>())
      .def("hasPoint", &Actor::hasPoint)
      .def("addAnimation", &Actor::addAnimation)
      .def("setAlignment", &Actor::setAlignment)
      .def("setMode", &Actor::setMode)
      .def("setPosition", &Actor::setPosition)
      .def("getPosition", &Actor::getPosition)
      .def("walk", &Actor::walk)
      .def("walkStraight", &Actor::walkStraight)
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
      .def(constructor<const Animation&>())
      .def(constructor<const std::string&>())
      .def("setBackground", &Scene::setBackground)
      .def("addActor", &Scene::addActor)
      .def("actorsMoved", &Scene::actorsMoved)
      ,

    class_<SDL_Event>("SDL_Event")
      .def_readonly("type", &SDL_Event::type)
      .def_readonly("motion", &SDL_Event::motion)
      .def_readonly("key", &SDL_Event::key)
      .def_readonly("user", &SDL_Event::user)
      ,

    class_<SDL_KeyboardEvent>("SDL_KeyboardEvent")
      .def_readonly("which", &SDL_KeyboardEvent::which)
      .def_readonly("state", &SDL_KeyboardEvent::state)
      .def_readonly("keysym", &SDL_KeyboardEvent::keysym)
      ,

    class_<SDL_MouseButtonEvent>("SDL_MouseButtonEvent")
      .def_readonly("which", &SDL_MouseButtonEvent::which)
      .def_readonly("button", &SDL_MouseButtonEvent::button)
      .def_readonly("state", &SDL_MouseButtonEvent::state)
      .def_readonly("x", &SDL_MouseButtonEvent::x)
      .def_readonly("y", &SDL_MouseButtonEvent::y)
      ,

    class_<SDL_MouseMotionEvent>("SDL_MouseMotionEvent")
      .def_readonly("which", &SDL_MouseMotionEvent::which)
      .def_readonly("state", &SDL_MouseMotionEvent::state)
      .def_readonly("x", &SDL_MouseMotionEvent::x)
      .def_readonly("y", &SDL_MouseMotionEvent::y)
      .def_readonly("xrel", &SDL_MouseMotionEvent::xrel)
      .def_readonly("yrel", &SDL_MouseMotionEvent::yrel)
      ,

    class_<SDL_UserEvent>("SDL_UserEvent")
      .def_readonly("code", &SDL_UserEvent::code)
      .def_readonly("data1", &SDL_UserEvent::data1)
      .def_readonly("data2", &SDL_UserEvent::data2)
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
      .property("x", &VirtualPosition::getX, &VirtualPosition::setX)
      .property("y", &VirtualPosition::getY, &VirtualPosition::setY)
      .def(self + self)
      .def(self - self)
      .def(-self)
      .def(self * double())
      .def(double() * self)
      .def(self == self)
      .def("nearestDirection", &VirtualPosition::nearestDirection)
      .def("length", &VirtualPosition::length)
      ,

    class_<Viewport>("Viewport")
      .def(constructor<>())
      .def(constructor<uint32_t, uint32_t>())
      .def(constructor<uint32_t, uint32_t, bool>())
      .def("setup", &Viewport::setup)
      .def("keepCentering", &Viewport::keepCentering)
  ];

  return 0;
}

