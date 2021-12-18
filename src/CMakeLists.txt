cmake_minimum_required(VERSION 3.14)
set (CMAKE_CXX_STANDARD 11)

# ==========================================================
# Example 
# ==========================================================

# set the project name and version
project(example VERSION 1.0)

# TODO : Project information 
configure_file(ProjectConfig.hpp.in ProjectConfig.hpp)

# add the example executable
add_executable(example main.cpp)
# add_executable(example main2.cpp)

# For projectconfig.hpp in build folder
target_include_directories(example PUBLIC "${PROJECT_BINARY_DIR}")

# ==========================================================
# Variables 
# ==========================================================

set(USE_32_BIT "FALSE" CACHE STRING "Create 32 bit library instead")
set(OUTPUT_STATIC "TRUE" CACHE STRING "Create static library instead")

#Find shared libraries in the same location as the executable
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")


# Use different library directory depending on the OS
if (WIN32)
    set(LIB_LOCATION "${PROJECT_SOURCE_DIR}/../libs/lib_windows")
elseif(UNIX)
    set(LIB_LOCATION "${PROJECT_SOURCE_DIR}/../libs/lib_linux")
endif()

set(INCLUDE_LOCATION "${PROJECT_SOURCE_DIR}/../include")


target_include_directories(example PUBLIC "${INCLUDE_LOCATION}")
set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")  #For surpressing goto warning

# ==========================================================
# ssGUI 
# ==========================================================
if("${OUTPUT_STATIC}")
    add_library(ssGUI STATIC
    "${PROJECT_SOURCE_DIR}/ssGUI/ssGUIManager.cpp"
)
else()
    add_library(ssGUI SHARED
    "${PROJECT_SOURCE_DIR}/ssGUI/ssGUIManager.cpp"
)
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/Backend")
add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/Backend/SFML")
add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/BaseClasses")
# add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/CompositeClasses")
add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/EventCallbacks")
add_subdirectory("${PROJECT_SOURCE_DIR}/ssGUI/Extensions")

target_include_directories(ssGUI PUBLIC "${INCLUDE_LOCATION}")

# ==========================================================
# SFML 
# ==========================================================

if(WIN32)
    # TODO : Implement this
    # set(SFML_STATIC_LIBRARIES TRUE)
    # set(SFML_DIR "${LIB_LOCATION}/SFML/cmake/SFML")
elseif(UNIX)

    if("${OUTPUT_STATIC}")
        ## If you want to link SFML statically
        set(SFML_STATIC_LIBRARIES TRUE)
        # In most cases better set in the CMake cache
        set(SFML_DIR "${LIB_LOCATION}/static/SFML/lib/cmake/SFML")
    else()
        # In most cases better set in the CMake cache
        set(SFML_DIR "${LIB_LOCATION}/shared/SFML/lib/cmake/SFML")
    endif()
endif()

find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
target_link_libraries(ssGUI PUBLIC sfml-network sfml-audio sfml-graphics sfml-window sfml-system)

# For disabling console (Probably mainly on windows)
# target_link_options(ssGUI PRIVATE -mwindows)

# Link the library
target_link_libraries(example PUBLIC ssGUI)