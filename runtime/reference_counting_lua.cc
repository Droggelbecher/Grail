
#include "reference_counting_lua.h"

template <typename T>
void pushReference(lua_State* L, T* target) {
  target->incrementeReferences();

  void *ptr = (void*)(dynamic_cast<ReferenceCounted*>(target));
  void *userdata = lua_newuserdata(L, sizeof(void*));
  (*(void**)userdata) = ptr;

  lua_newtable(L);
  lua_pushcfunction(L, &onReferenceCollected);

  lua_setmetatable(L, -2);
}


int onReferenceCollected(lua_State* L) {
  // only parameter: a full userdata object
  void *userdata = lua_touserdata(L, 1);
  void *ptr = *((void**)userdata);
  ((ReferenceCounted*)ptr)->decrementReferences();
  return 0;
}


