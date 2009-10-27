
link_directories(lib)
include_directories(../lib .. /usr/include/lua5.1)

add_executable(grail_runtime
  interpreter.cc
  lua_utils_impl.cc
  main.cc
  wrappings.cc
)

set(LIBS grail lua5.1 SDL SDL_image png)

target_link_libraries(grail_runtime ${LIBS})

