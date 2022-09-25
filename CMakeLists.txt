cmake_minimum_required(VERSION 3.14)
# cmake_minimum_required(VERSION 3.24)

set (CMAKE_CXX_STANDARD 11)

# For Clang to do parsing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(ssGUI)

# ==========================================================
# Variables 
# ==========================================================

set(ssGUI_OUTPUT_STATIC TRUE CACHE BOOL "Use and create static library instead")
set(ssGUI_BUILD_TESTS TRUE CACHE BOOL "Build tests")
set(ssGUI_BUILD_EXAMPLES TRUE CACHE BOOL "Build examples")
set(ssGUI_USE_LOCAL_SFML TRUE CACHE BOOL "Use the SFML that comes with ssGUI")

# This might not be needed 
set(ssGUI_SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml/include" CACHE PATH "Provide the directory of ")

#Find shared libraries in the same location as the executable
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")

# target_include_directories(example PUBLIC "${INCLUDE_LOCATION}")
set(CMAKE_CXX_FLAGS "-fpermissive -std=c++0x")  #For surpressing goto warning

# ==========================================================
# clip 
# ==========================================================

# Disable clip examples and tests
set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

# In case that you have ${PNG_LIBRARY} set to support copy/paste images on Linux
#set(CLIP_X11_PNG_LIBRARY "${PNG_LIBRARY}")

# Add clip subdirectory to compile the library
add_subdirectory("${PROJECT_SOURCE_DIR}/External/clip")

target_include_directories(clip PUBLIC "${PROJECT_SOURCE_DIR}/External/clip")

# ==========================================================
# SFML 
# ==========================================================

if(ssGUI_USE_LOCAL_SFML)

    # Use different library directory depending on the static or shared
    set(LIB_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml")
    if (ssGUI_OUTPUT_STATIC)
        set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml/static/SFML/lib/cmake/SFML")
    else()
        set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml/shared/SFML/lib/cmake/SFML")
    endif()

    # set(INCLUDE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    # message("${SFML_DIR}")

    # target_link_options(ssGUI PRIVATE -mwindows) # uncomment to disabling console on Windows
    if(ssGUI_OUTPUT_STATIC)
        set(SFML_STATIC_LIBRARIES TRUE)
    endif()

    # Import SFML
    find_package(SFML 3 COMPONENTS System Window Graphics Network Audio REQUIRED)

endif()

# ==========================================================
# ssGUI 
# ==========================================================
if(ssGUI_OUTPUT_STATIC)
    add_library(ssGUI STATIC
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Factory.cpp"
)
else()
    add_library(ssGUI SHARED
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Factory.cpp"
)
endif()

# If you are on 32 bit linux, uncomment this
# set (CMAKE_CXX_FLAGS "-m32")
# set (CMAKE_C_FLAGS "-m32")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/SFML")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/DataClasses")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/EventCallbacks")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Extensions")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/GUIObjectClasses")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/GUIObjectClasses/CompositeClasses")

# ssGUI include directory
target_include_directories(ssGUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
                                        "${ssGUI_SFML_INCLUDE_DIR}")                            # This might not be needed

# Get the include directories for the target.
get_target_property(LIBA_INCLUDES ssGUI INCLUDE_DIRECTORIES)

# Construct the compiler string for the include directories.
foreach(dir ${LIBA_INCLUDES} ${LIBB_INCLUDES})
    string(APPEND INCLUDE_COMPILER_STRING "${dir}\n")
endforeach()
message("${INCLUDE_COMPILER_STRING}")

# Linking clip to ssGUI
target_link_libraries(ssGUI PUBLIC clip)

# Linking SFML to ssGUI
target_link_libraries(ssGUI PUBLIC SFML::Network SFML::Audio SFML::Graphics SFML::Window SFML::System)

# Copy resources for the library 
file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}") 

# ==========================================================
# Building tests and examples 
# ==========================================================

if(ssGUI_BUILD_TESTS)

    # set(ssGUI_TEST_CPP_FILES "" STRING)
    file(GLOB ssGUI_TEST_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Src/Tests/*.cpp")

    # message("${ssGUI_TEST_CPP_FILES}")

    foreach(CUR_SOURCE_PATH ${ssGUI_TEST_CPP_FILES})

        # Get the file name without path and extension
        get_filename_component(CUR_NAME "${CUR_SOURCE_PATH}" NAME_WE)  
        
        # message("${CUR_SOURCE_PATH}\n")
        # message("${CUR_NAME}\n")
        # message("${ssGUI_TEST_CPP_FILES}")

        # add the executable
        add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")

        # Link ssGUI to executable
        target_link_libraries("${CUR_NAME}" PUBLIC ssGUI)

        # Sub-directory for build
        set_target_properties("${CUR_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Tests")
    endforeach(CUR_SOURCE_PATH "$[ssGUI_TEST_CPP_FILES}")

    # Copy resources for the Tests 
    file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Tests") 

endif()


if(ssGUI_BUILD_EXAMPLES)

    # set(ssGUITestFiles "" STRING)
    file(GLOB ssGUI_EXAMPLE_CPP_FILES "${CMAKE_CURRENT_SOURCE_DIR}/Src/Examples/*.cpp")
    foreach(CUR_SOURCE_PATH ${ssGUI_EXAMPLE_CPP_FILES})

        # Get the file name without path and extension
        get_filename_component(CUR_NAME "${CUR_SOURCE_PATH}" NAME_WE)  

        # add the executable
        add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")

        # Link ssGUI to executable
        target_link_libraries("${CUR_NAME}" PUBLIC ssGUI)

        # Sub-directory for build
        set_target_properties("${CUR_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Examples")

    endforeach()

    # Copy resources for the Examples 
    file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Examples") 

endif()