
set(GRAIL_FILES
  animation.cc
  game.cc
  mainloop.cc
  registry.cc
  resource_manager.cc
  shortcuts.cc
  unittest.cc
  user_interface.cc
  utils.cc
  vector2d.cc
  viewport.cc
)

add_library(grail ${GRAIL_FILES})


include_directories(. ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
add_executable(run_unittests  run_unittests.cc vector2d_impl.cc)
set(LIBS grail ${PNG_LIBRARIES} ${SDLIMAGE_LIBRARY} ${SDL_LIBARY} ${LUA_LIBRARIES})
target_link_libraries(run_unittests ${LIBS})



