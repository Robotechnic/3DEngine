project(cubes)

add_executable(
	cubes
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(cubes PRIVATE 3Dengine)