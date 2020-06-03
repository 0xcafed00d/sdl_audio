cmake_minimum_required(VERSION 3.17.0)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

project (sdlaudio)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
	sdlaudio
	src/audio.cpp
	src/main.cpp
)

target_link_libraries (
	sdlaudio 
	${SDL2_LIBRARIES}
)  