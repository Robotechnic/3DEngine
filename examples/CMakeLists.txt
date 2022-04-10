include(${CMAKE_CURRENT_LIST_DIR}/cubes/CMakeLists.txt)
include(${CMAKE_CURRENT_LIST_DIR}/objLoader/CMakeLists.txt)

# if an exemple project is build, add executable to .gitignore
if (${CMAKE_PROJECT_NAME} STREQUAL "")

else()
	message("Building target ${CMAKE_PROJECT_NAME}")
	file(WRITE "${CMAKE_CURRENT_LIST_DIR}/${CMAKE_PROJECT_NAME}/.gitignore" "${CMAKE_PROJECT_NAME}")
endif()