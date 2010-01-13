
link_directories(${GRAIL_LIBDIR})
include_directories(.. ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

#add_executable(wrapc wrapc.cc)

#add_custom_command(
#  OUTPUT wrapper_definitions.cc
#  COMMAND wrapc wrappers.def > wrapper_definitions.cc
#  DEPENDS wrappers.def wrapc
#  MAIN_DEPENDENCY wrappers.def
#)

add_executable(grail_runtime
  interpreter.cc
  lua_utils_impl.cc
  ../lib/vector2d_impl.cc
  ../lib/reference_counting_impl.cc
  main.cc
  lua_bindings.cc
)

set(LIBS grail ${PNG_LIBRARIES} ${SDLIMAGE_LIBRARY} ${SDL_LIBRARY} ${LUA_LIBRARIES} ${LUABIND_LIBRARY})

target_link_libraries(grail_runtime ${LIBS})

