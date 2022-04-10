project(objLoader)

add_executable(
	objLoader
	${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

target_link_libraries(objLoader PRIVATE 3Dengine)