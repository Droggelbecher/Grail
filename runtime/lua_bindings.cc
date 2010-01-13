
#include <iostream>
#include <luabind/luabind.hpp>
#include "lua_bindings.h"

void test_luabind() {
  std::cout << "this is test_luabind()" << std::endl;
}

extern "C" int init(lua_State* L) {
  using namespace luabind;

  open(L);

  module(L) [
    def("test_luabind", &test_luabind)
  ];

  return 0;
}

