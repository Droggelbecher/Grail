
#include "lua_chunk.h"

int chunkWriterCallback(lua_State* L, const void* p, size_t sz, void* data) {
  return ((LuaChunk*)data)->writerCallback(L, p, sz, data);
}

const char* chunkReaderCallback(lua_State* L, void* data, size_t* sz) {
  return ((LuaChunk*)data)->readerCallback(L, data, sz);
}

