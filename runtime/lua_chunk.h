
#ifndef LUA_CHUNK_H
#define LUA_CHUNK_H

#include <vector>
#include <lua.hpp>
#include <algorithm>
#include "lib/resource_manager.h"
#include "lua_exception.h"

int chunkWriterCallback(lua_State* L, const void* p, size_t sz, void* data);
const char* chunkReaderCallback(lua_State* L, void* data, size_t* sz);

class LuaChunk {
    std::vector<char> buffer;
    size_t readerPosition;
    std::string name;

  public:
    LuaChunk() : readerPosition(0), name("unnamed chunk of lua code") { }
    LuaChunk(const std::string& name) : readerPosition(0), name(name) { }
    LuaChunk(grail::Resource& resource) : readerPosition(0), name(resource.getPath()) {
      buffer.resize(resource.getDataSize());
      std::copy(resource.getData(), resource.getData() + resource.getDataSize(), buffer.begin());
    }

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

    static LuaChunk* dump(lua_State* L, int n) {
      LuaChunk *chunk = new LuaChunk();
      lua_pushvalue(L, n);
      lua_dump(L, &chunkWriterCallback, (void*)chunk);
      lua_pop(L, 1);
      return chunk;
    }

    void load(lua_State* L) {
      int r = lua_load(L, &chunkReaderCallback, (void*)this, name.c_str());
      if(r != 0) {
        throw LuaException(L);
      }
    }
};

#endif // LUA_CHUNK_H

