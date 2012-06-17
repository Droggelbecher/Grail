# vim: set noexpandtab:
# -Ddebug=1 for debug build

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(GRAIL_VERSION "0.1" CACHE STRING "Version")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

set(USE_VISUALIZATION ON CACHE BOOL "Compile-in visualizaton support (currently requires USE_OPENGL to be OFF to work)")
set(USE_OPENGL OFF CACHE BOOL "Compile with OpenGL support (better performance)")
set(DEBUG ON CACHE BOOL "Compile in debug mode")

project(grail)

set(CMAKE_C_FLAGS "$ENV{CFLAGS} -Wl,--no-as-needed -lopenal -lalure -lalut")
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -Wl,--no-as-needed -lopenal -lalure -lalut")


find_package(Lua51 REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(SDL_ttf REQUIRED)
find_package(SDL_gfx REQUIRED)

if(USE_OPENGL)
	find_package(OpenGL)
	#if(OPENGL_FOUND)
	add_definitions(-DWITH_OPENGL)
	set(USE_OPENGL true)
endif(USE_OPENGL)
find_package(PNG REQUIRED)
find_package(Luabind REQUIRED)

set(Boost_USE_MULTITHREADED OFF)
find_package(Boost REQUIRED COMPONENTS system filesystem regex)

find_package(OpenAL REQUIRED)
#find_package(Alure REQUIRED) alure find macro still missing

if(DEBUG)
	set(CMAKE_C_FLAGS "$ENV{CFLAGS} -g -O0 -Wall -ansi -pedantic -pipe -Wl,--no-as-needed -lopenal -lalure -lalut")
	set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -g -O0 -Wall -ansi -pedantic -pipe -Wl,--no-as-needed -lopenal -lalure -lalut")
	add_definitions(-DDEBUG)
endif(DEBUG)

if(USE_VISUALIZATION)
	add_definitions(-DWITH_VISUALIZATION)
endif(USE_VISUALIZATION)

set(GRAIL_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(GRAIL_RUNTIMEDIR "${CMAKE_CURRENT_SOURCE_DIR}/runtime")

add_subdirectory(${GRAIL_LIBDIR})
add_subdirectory(${GRAIL_RUNTIMEDIR})

message("CFLAGS:${CMAKE_C_FLAGS}")
message("CXXFLAGS:${CMAKE_CXX_FLAGS}")

