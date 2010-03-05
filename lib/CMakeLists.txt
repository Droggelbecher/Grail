
set(GRAIL_FILES
  actor.cc
  animation.cc
  action_text.cc
  area.cc
  audio.cc
  blit_cached.cc
  debug.cc
  event.cc
  font.cc
  game.cc
  ground.cc
  line.cc
  mainloop.cc
  polygon.cc
  resource_manager.cc
  scene.cc
  sdlutils.cc
  shortcuts.cc
  sprite.cc
  text.cc
  unittest.cc
  user_interface.cc
  user_interface_element.cc
  utils.cc
  vector2d.cc
  viewport.cc
)

# Library

add_library(grail SHARED ${GRAIL_FILES})

# Unit tests for library

include_directories(${SDLTTF_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${SDLGFX_INCLUDE_DIR} ${BOOST_INCLUDE_DIR} ${LUA_INCLUDE_DIR})
add_executable(run_unittests  run_unittests.cc vector2d_impl.cc)
set(LIBS
  grail
  ${Boost_LIBRARIES}
  ${SDLGFX_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDLMIXER_LIBRARY} ${SDL_LIBRARY}
  ${PNG_LIBRARIES}
  ${LUA_LIBRARIES}
)
target_link_libraries(run_unittests ${LIBS})

