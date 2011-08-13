
link_directories(${GRAIL_LIBDIR})
include_directories(.. ${SDLTTF_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${LUA_INCLUDE_DIR})

add_executable(grail_runtime
  ../lib/vector2d_impl.cc
  ../lib/polygon_impl.cc
  interpreter.cc
  lua_bindings.cc
  main.cc
  network_interface.cc
)

set(LIBS
  grail
  ${SDLGFX_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDL_LIBRARY}
  ${PNG_LIBRARIES}
  ${LUA_LIBRARIES} ${LUABIND_LIBRARY}
  ${Boost_LIBRARIES}
  alure #till FindAlure exists
  )

if(OPENGL_FOUND)
	set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
	message("OpenGL found")
	message("  Include dir: ${OPENGL_INCLUDE_DIR}")
	message("  Libraries: ${OPENGL_LIBRARIES}")
else(OPENGL_FOUND)
	message("OpenGL not found, building without!")
endif(OPENGL_FOUND)

target_link_libraries(grail_runtime ${LIBS})

