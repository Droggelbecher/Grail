
#include <iostream>

#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include <SDL/SDL.h>
#include <lua.hpp>

#include "lib/actor.h"
#include "lib/animation.h"
#include "lib/game.h"
#include "lib/image.h"
#include "lib/resource_manager.h"
#include "lib/resource_manager.h"
#include "lib/scene.h"
#include "lib/sprite.h"
#include "lib/text.h"
#include "lib/user_interface.h"
#include "lib/user_interface_element.h"
#include "lib/vector2d.h"
#include "lib/font.h"
#include "lib/viewport.h"
#include "lib/action_text.h"
#include "lua_bindings.h"
#include "lua_converter.h"
#include "lib/sdl_exception.h"

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

    class_<UserInterface, UserInterfaceWrapper, UserInterface::Ptr>("UserInterface")
      .def(constructor<>())
      .def("handleEvent", &UserInterface::handleEvent, &UserInterfaceWrapper::default_handleEvent)
      .def("addElement", &UserInterface::addElement)
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

