add_library(glad "${CMAKE_CURRENT_LIST_DIR}/src/glad.c")

target_include_directories(glad PUBLIC "${CMAKE_CURRENT_LIST_DIR}/include")