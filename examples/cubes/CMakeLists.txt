project(cubes)

add_executable(
	cubes
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(cubes PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(cubes PRIVATE 3Dengine)