
link_directories(${GRAIL_LIBDIR})
include_directories(.. ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(wrapc wrapc.cc)

add_custom_command(
  OUTPUT wrapper_definitions.cc
  COMMAND wrapc wrappers.def > wrapper_definitions.cc
  DEPENDS wrappers.def wrapc
  MAIN_DEPENDENCY wrappers.def
)

add_executable(grail_runtime
  interpreter.cc
  lua_utils_impl.cc
  ../lib/vector2d_impl.cc
  main.cc
  wrapper_definitions.cc
  handcrafted_wrappers.cc
)

set(LIBS grail ${PNG_LIBRARIES} ${SDLIMAGE_LIBRARY} ${SDL_LIBARY} ${LUA_LIBRARIES})

target_link_libraries(grail_runtime ${LIBS})

