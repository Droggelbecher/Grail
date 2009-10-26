
cmake_minimum_required(VERSION 2.6)

if(debug)
  set(CMAKE_C_FLAGS "-g -O0 -Wall -pipe")
  set(CMAKE_CXX_FLAGS "-g -O0 -Wall -pipe")
endif(debug)

project(grail)
add_subdirectory(lib)
add_subdirectory(runtime)

