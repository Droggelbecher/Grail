# -Ddebug=1 for debug build

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)
set(GRAIL_VERSION "0.1" CACHE STRING "Version")

project(grail)

set(CMAKE_C_FLAGS $ENV{CFLAGS})
set(CMAKE_CXX_FLAGS $ENV{CXXFLAGS})

find_package(Lua51 REQUIRED)
find_package(SDL REQUIRED)
find_package(SDL_image REQUIRED)
find_package(PNG REQUIRED)
find_package(LUABIND REQUIRED)

if(debug)
  set(CMAKE_C_FLAGS "-g -O0 -Wall -pipe")
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -pipe")
  add_definitions(-DDEBUG)
endif(debug)

set(GRAIL_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/lib")
set(GRAIL_RUNTIMEDIR "${CMAKE_CURRENT_SOURCE_DIR}/runtime")

add_subdirectory(${GRAIL_LIBDIR})
add_subdirectory(${GRAIL_RUNTIMEDIR})

message("CFLAGS:${CMAKE_C_FLAGS}")
message("CXXFLAGS:${CMAKE_CXX_FLAGS}")

