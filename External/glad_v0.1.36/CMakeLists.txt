
if(WIN32)
	add_library(glad "${CMAKE_CURRENT_LIST_DIR}/src/glad.c"
				"${CMAKE_CURRENT_LIST_DIR}/src/glad_wgl.c")
else()
	add_library(glad "${CMAKE_CURRENT_LIST_DIR}/src/glad.c"
				"${CMAKE_CURRENT_LIST_DIR}/src/glad_glx.c")
endif()

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")