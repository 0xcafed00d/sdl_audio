cmake_minimum_required(VERSION 3.17.0)

project (sdlaudio)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(
	${CMAKE_PROJECT_NAME}
	src/audio.cpp
	src/main.cpp
)

target_link_libraries (
	${CMAKE_PROJECT_NAME}
	${SDL2_LIBRARIES}
)  
