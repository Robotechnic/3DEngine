project(objLoader)

add_executable(
	objLoader
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

set_target_properties(objLoader PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(objLoader PRIVATE 3Dengine)