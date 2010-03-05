
link_directories(${GRAIL_LIBDIR})
include_directories(.. ${SDLTTF_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${SDLGFX_INCLUDE_DIR} ${SDLMIXER_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(grail_runtime
  ../lib/vector2d_impl.cc
  interpreter.cc
  lua_bindings.cc
  main.cc
  network_interface.cc
)

set(LIBS
  grail
  ${SDLGFX_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY}
  ${PNG_LIBRARIES}
  ${LUA_LIBRARIES} ${LUABIND_LIBRARY}
  ${Boost_LIBRARIES}
  )

target_link_libraries(grail_runtime ${LIBS})

