# vim: set noexpandtab:

set(GRAIL_FILES
	action_text.cc
	actor.cc
	animation.cc
	area.cc
	audio.cc
	blit_cached.cc
	debug.cc
	dialog_line.cc
	dialog_frontend.cc
	dialog_frontend_text.cc
	dialog_frontend_speech_bubbles.cc
	dialog_frontend_subtitle.cc
	event.cc
	font.cc
	game.cc
	ground.cc
	line.cc
	mainloop.cc
	polygon.cc
	polygon_impl.cc
	resource_manager.cc
	scene.cc
	sdlutils.cc
	sound_task.cc
	shortcuts.cc
	sprite.cc
	surface.cc
	task.cc
	text.cc
	unittest.cc
	user_interface.cc
	user_interface_element.cc
	utils.cc
	vector2d.cc
	viewport.cc
	wait_task.cc
)

# Library

add_library(grail SHARED ${GRAIL_FILES})

# Unit tests for library

include_directories(${SDLTTF_INCLUDE_DIR} ${SDLIMAGE_INCLUDE_DIR} ${SDL_INCLUDE_DIR} ${BOOST_INCLUDE_DIR} ${LUA_INCLUDE_DIR} ${OPENAL_INCLUDE_DIR}
	/usr/local/include/AL # till FindAlure exists
	)
add_executable(run_unittests	run_unittests.cc vector2d_impl.cc)
set(LIBS
	grail
	${Boost_LIBRARIES}
	${SDLGFX_LIBRARY} ${SDLTTF_LIBRARY} ${SDLIMAGE_LIBRARY} ${SDL_LIBRARY}
	${PNG_LIBRARIES}
	${LUA_LIBRARIES}
	${OPENAL_LIBRARY}
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

target_link_libraries(run_unittests ${LIBS})

execute_process(COMMAND run_unittests)

#enable_testing()
#add_test(
#	NAME unittests
#	WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}"
#	COMMAND run_unittests
#)

