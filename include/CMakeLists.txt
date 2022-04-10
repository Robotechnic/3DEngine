add_library(math INTERFACE)
target_include_directories(math INTERFACE ${CMAKE_CURRENT_LIST_DIR}/math)

add_library(scene INTERFACE)
target_include_directories(scene INTERFACE ${CMAKE_CURRENT_LIST_DIR}/scene)

add_library(shapes INTERFACE)
target_include_directories(shapes INTERFACE ${CMAKE_CURRENT_LIST_DIR}/shapes)

list(APPEND app_include
	${CMAKE_CURRENT_LIST_DIR}
)