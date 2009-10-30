
link_directories(${GRAIL_LIBDIR})
include_directories(${GRAIL_LIBDIR} .. ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(grail_runtime
  interpreter.cc
  lua_utils_impl.cc
  main.cc
  wrappings.cc
)

set(LIBS grail ${PNG_LIBRARIES} ${SDLIMAGE_LIBRARY} ${SDL_LIBARY} ${LUA_LIBRARIES})

target_link_libraries(grail_runtime ${LIBS})

