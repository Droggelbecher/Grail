
#ifndef LUA_UTILS_H
#define LUA_UTILS_H

#include <string>
#include <vector>
#include <stdint.h>
#include <cassert>

#include <lua.hpp>
#include "lib/vector2d.h"
#include "lua_exception.h"

/**
 * luaGet<Type, int N>(lua_State* L)
 *
 * Get data from stack at position N as type Type
 */

// Use a struct with a single method because classes/struct templates can be partially
// specialized, function templates can not
// (Idea from: http://www.gotw.ca/publications/mill17.htm)
template <typename T> struct _LuaGet { static T get(lua_State* L, int n); };
template <typename T> struct _LuaGet<T*> { static T* get(lua_State* L, int n); };
template <typename T> struct _LuaGet<T&> { static T& get(lua_State* L, int n); };

template <typename T> T luaGet(lua_State* L, int n);

template <typename T> struct _LuaPush { static int push(lua_State* L, T obj); };
template <typename T> struct _LuaPush<T*> { static int push(lua_State* L, T* obj); };
template <typename T> struct _LuaPush<T&> { static int push(lua_State* L, T& obj); };

template <typename T> int luaPush(lua_State* L, T obj);

template <typename T>
int luaPushReference(lua_State* L, T* obj, std::string className);

#define L_STACK(L) int __lua_stack_assert = lua_gettop(L);
#define L_RETURN(L, N) assert(lua_gettop(L) == (__lua_stack_assert + (N)));
#define L_NOTNIL(L) assert(!lua_isnil(L, -1))

int chunkWriterCallback(lua_State* L, const void* p, size_t sz, void* data);
const char* chunkReaderCallback(lua_State* L, void* data, size_t* sz);

class Chunk {
    std::vector<char> buffer;
    size_t readerPosition;

  public:
    Chunk() : readerPosition(0) { }

    int writerCallback(lua_State* L, const void* p, size_t sz, void* data) {
      buffer.reserve(buffer.size() + sz);
      for(char *i = (char*)p; i<((char*)p+sz); i++) {
        buffer.push_back(*i);
      }
      return 0;
    }

    const char* readerCallback(lua_State* L, void* data, size_t* sz) {
      *sz = buffer.size() - readerPosition;
      if(*sz <= 0) {
        *sz = 0;
        return "\0";
      }
      const char* r = &(buffer.front()) + readerPosition;
      readerPosition += *sz;

      return r;
    }

    static Chunk* dump(lua_State* L, int n) {
      Chunk *chunk = new Chunk();
      lua_pushvalue(L, n);
      lua_dump(L, &chunkWriterCallback, (void*)chunk);
      lua_pop(L, 1);
      return chunk;
    }

    void load(lua_State* L) {
      int r = lua_load(L, &chunkReaderCallback, (void*)this, "User defined callback");
      if(r != 0) {
        throw LuaException(L);
      }
    }
};

#endif // LUA_UTILS_H

