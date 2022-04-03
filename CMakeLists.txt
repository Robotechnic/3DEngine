cmake_minimum_required(VERSION 3.22)
project(3Dengine VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

include_directories(
	src
)

add_executable(
	3Dengine
	src/math/matrix4.cpp
	src/shapes/triangle.cpp
	src/shapes/shape.cpp
	src/shapes/cube.cpp
	src/scene/scene.cpp
	src/main.cpp
)

target_link_libraries(
	3Dengine
	sfml-graphics
	sfml-window
	sfml-system
)