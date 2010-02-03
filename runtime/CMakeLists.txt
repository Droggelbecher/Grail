
link_directories(${GRAIL_LIBDIR})
include_directories(.. ${SDLTTF_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(grail_runtime
  interpreter.cc
  ../lib/vector2d_impl.cc
  main.cc
  lua_bindings.cc
)

set(LIBS grail ${PNG_LIBRARIES} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDL_LIBRARY} ${LUA_LIBRARIES} ${LUABIND_LIBRARY})

target_link_libraries(grail_runtime ${LIBS})

