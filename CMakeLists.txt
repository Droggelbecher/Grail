# -Ddebug=1 for debug build

cmake_minimum_required(VERSION 2.6)

project(grail)

SET ( CMAKE_C_FLAGS $ENV{CFLAGS} )
SET ( CMAKE_CXX_FLAGS $ENV{CXXFLAGS} )

find_package ( Lua51 REQUIRED )
find_package ( SDL REQUIRED )
find_package ( SDL_image REQUIRED)
find_package ( PNG REQUIRED )

if(debug)
  set(CMAKE_C_FLAGS "-g -O0 -Wall -pipe")
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -pipe")
  add_definitions ( -DDEBUG )
endif(debug)

SET ( GRAIL_LIBDIR "${CMAKE_CURRENT_SOURCE_DIR}/lib" )
SET ( GRAIL_RUNTIMEDIR "${CMAKE_CURRENT_SOURCE_DIR}/runtime" )

add_subdirectory(${GRAIL_LIBDIR})
add_subdirectory(${GRAIL_RUNTIMEDIR})

MESSAGE ( "CFLAGS:${CMAKE_C_FLAGS}" )
MESSAGE ( "CXXFLAGS:${CMAKE_CXX_FLAGS}" )
