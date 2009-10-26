
link_directories(lib)
include_directories(../lib .. /usr/include/lua5.1)

add_executable(grail_runtime main.cc interpreter.cc)

set(LIBS grail lua5.1 SDL SDL_image png)

target_link_libraries(grail_runtime ${LIBS})

