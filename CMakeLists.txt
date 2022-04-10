cmake_minimum_required(VERSION 3.22)
project(3Dengine VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

include(${PROJECT_SOURCE_DIR}/src/CMakeLists.txt)
include(${PROJECT_SOURCE_DIR}/include/CMakeLists.txt)

add_library(
	3Dengine
	${app_src}
)

target_include_directories(3Dengine PUBLIC
	${app_include}
)

target_link_libraries(
	3Dengine
	sfml-graphics
	sfml-window
	sfml-system
)

include(${PROJECT_SOURCE_DIR}/examples/CMakeLists.txt)