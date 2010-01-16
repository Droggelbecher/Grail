
#include <iostream>
#include <luabind/luabind.hpp>
#include <luabind/operator.hpp>
#include "lua_bindings.h"

#include "lib/game.h"
#include "lib/resource_manager.h"
#include "lib/user_interface.h"
#include "lib/viewport.h"
#include "lib/scene.h"
#include "lib/vector2d.h"

void test_luabind() {
  grail::cdbg << "If you can read this, luabind works" << std::endl;
}

extern "C" int init(lua_State* L) {
  using namespace luabind;
  using namespace grail;

  open(L);

  module(L, "grail")[
    def("test_luabind", &test_luabind),
    def("getGame", &GameWrapper::getInstance),

    class_<GameWrapper>("Game")
      .scope[
        def("getInstance", &GameWrapper::getInstance)
        ]
      .def("getViewport", &GameWrapper::getViewport)
      .def("getCurrentScene", &GameWrapper::getCurrentScene)
      .def("getResourceManager", &GameWrapper::getResourceManager)
      .def("getUserInterface", &GameWrapper::getUserInterface)
      .def("setUserInterface", &GameWrapper::setUserInterface)
      .def("initChapter", &GameWrapper::initChapter)
      .def("setInitChapterCallback", &GameWrapper::setInitChapterCallback)
      .def("runChapter", &GameWrapper::runChapter)
      ,

    //class_<Image>("Image")
    //  .def(constructor<std::string>()),
      //.def("getSize", &Image::getSize),

    class_<ResourceManager>("ResourceManager"),

    class_<Scene>("Scene")
      .def(constructor<>())
      ,

    class_<UserInterface>("UserInterface")
      .def(constructor<>())
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


  ];

  return 0;
}

