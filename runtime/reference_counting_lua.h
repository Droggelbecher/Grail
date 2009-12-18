
#ifndef REFERENCE_COUNTING_LUA_H
#define REFERENCE_COUNTING_LUA_H

#include <lua.hpp>
#include "lib/reference_counting.h"

template <typename T>
void pushReference(lua_State* L, T* target);

int onReferenceCollected(lua_State* L);

#endif // REFERENCE_COUNTING_LUA_H

