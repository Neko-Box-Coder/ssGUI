cmake_minimum_required(VERSION 3.14)
# cmake_minimum_required(VERSION 3.24)

set (CMAKE_CXX_STANDARD 11)

# For Clang to do parsing
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set the project name
project(ssGUI)

# Set use folder name for IDEs such as Visual Studio
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# ==========================================================
# Variables 
# ==========================================================

# Build Variables
set(ssGUI_OUTPUT_STATIC ON CACHE BOOL "Use and create static library instead")
set(ssGUI_BUILD_TESTS ON CACHE BOOL "Build tests")
set(ssGUI_BUILD_EXAMPLES ON CACHE BOOL "Build examples")
set(ssGUI_MAIN_BACKEND_DESCRIPTION "The main backend that will be used by ssGUI")
set(ssGUI_FONT_BACKEND_DESCRIPTION "The font backend that will be used by ssGUI")
set(ssGUI_IMAGE_BACKEND_DESCRIPTION "The image backend that will be used by ssGUI")


# Backend options variable
if(WIN32)
    set(ssGUI_MAIN_BACKEND "Win32_OpenGL3_3" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")
    set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS "Win32_OpenGL3_3" "SFML" "Template")     # Allow dropdown option
# Linux
else()
    set(ssGUI_MAIN_BACKEND "X11_OpenGL3_3" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")
    set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS "X11_OpenGL3_3" "SFML" "Template")                       # Allow dropdown option
endif()

# Show / unshow SFML options depending if SFML backend is selected or not
if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
    set(ssGUI_USE_LOCAL_SFML ON CACHE BOOL "Use the SFML that comes with ssGUI")
    set(ssGUI_USE_SFML_TIME OFF CACHE BOOL "Use SFML time instead of chrono time")
    
    set(ssGUI_FONT_BACKEND "SFML" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
    set(ssGUI_IMAGE_BACKEND "SFML" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")
    set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS "SFML" "FreeType" "Template")            # Allow dropdown option
    set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "SFML" "stb_image" "Template")          # Allow dropdown option

    # This might not be needed 
    set(ssGUI_SFML_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml/include" CACHE PATH "Provide the directory of ")
    set (CMAKE_CXX_STANDARD 17)
else()
    unset(ssGUI_USE_LOCAL_SFML CACHE)
    unset(ssGUI_USE_SFML_TIME CACHE)
endif()

if(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
    set(ssGUI_FONT_BACKEND "FreeType" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
    set(ssGUI_IMAGE_BACKEND "stb_image" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

    set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS "FreeType" "Template")            # Allow dropdown option
    set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "stb_image" "Template")                      # Allow dropdown option
endif()

if(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
    set(ssGUI_FONT_BACKEND "FreeType" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
    set(ssGUI_IMAGE_BACKEND "stb_image" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

    set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS "FreeType" "Template")            # Allow dropdown option
    set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "stb_image" "Template")                      # Allow dropdown option
endif()

if(ssGUI_MAIN_BACKEND STREQUAL "Template")
    set(ssGUI_FONT_BACKEND "Template" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
    set(ssGUI_IMAGE_BACKEND "Template" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

    set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS "Template")            # Allow dropdown option
    set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "Template")           # Allow dropdown option
endif()

# Debug Variables
set(ssGUI_USE_DEBUG OFF CACHE BOOL "Prints extra debug information")
set(ssGUI_DEBUG_STATE OFF CACHE BOOL "Prints each stage in ssGUIManager")
set(ssGUI_REFRESH_CONSOLE OFF CACHE BOOL "Clears the console at the end of each frame")
set(ssGUI_SLOW_UPDATE OFF CACHE BOOL "Fps will be set to 1 for easier debugging")

# Overriding ssLogger options
set(ssLOG_BUILD_TESTS "FALSE" CACHE BOOL "If true, this will build tests")
set(ssLOG_CALL_STACK "FALSE" CACHE BOOL "If true, this will build tests")
set(ssLOG_THREAD_SAFE "FALSE" CACHE BOOL "Use std::thread and ensure thread safety for all logged functions")

# Dividers
set(ssGUI@_CMAKE_VARIABLES_START============================= "=================== I am just a divider :D ==================" CACHE STRING "Divider") 
set(ssGUI__CMAKE_VARIABLES_END=============================== "=================== I am just a divider :D ==================" CACHE STRING "Divider") 

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

set_target_properties(clip PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio

# ==========================================================
# SFML 
# ==========================================================

if(ssGUI_USE_LOCAL_SFML AND ssGUI_MAIN_BACKEND STREQUAL "SFML")

    set(SFML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/External/sfml/lib/cmake/SFML")

    # set(INCLUDE_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/../include")
    # message("${SFML_DIR}")

    # target_link_options(ssGUI PRIVATE -mwindows) # uncomment to disabling console on Windows
    if(ssGUI_OUTPUT_STATIC)
        set(SFML_STATIC_LIBRARIES TRUE)
    endif()

    # Import SFML
    find_package(SFML 3 COMPONENTS System Window Graphics Network Audio REQUIRED)

    # TODO: Check if this works
    set_target_properties(SFML PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
endif()

# ==========================================================
# ssLogger 
# ==========================================================

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/ssLogger" "${CMAKE_CURRENT_BINARY_DIR}/ssLogger")

set_target_properties(ssLogger PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio

# ==========================================================
# FreeType
# ==========================================================

if(ssGUI_FONT_BACKEND STREQUAL "FreeType")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/External/freetype" "${CMAKE_CURRENT_BINARY_DIR}/freetype")
    unset(FT_DISABLE_HARFBUZZ CACHE)
    set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "")  # Disabling FT_DISABLE_HARFBUZZ by default as it is causing errors
    set_target_properties(freetype PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
endif()

# ==========================================================
# stb_image
# ==========================================================

if(ssGUI_IMAGE_BACKEND STREQUAL "stb_image")
    add_library(ssGUI_stb_image INTERFACE)
    
    #target_sources( stb_image INTERFACE 
    #                FILE_SET HEADERS
    #                BASE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image"
    #                FILES "${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image/stb_image.h")
    target_include_directories(ssGUI_stb_image INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/External/stb_image")
    set_target_properties(ssGUI_stb_image PROPERTIES FOLDER External)   # Set folder name for IDEs such as Visual Studio
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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/utf-8)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ssGUI PRIVATE -Wall -Wno-return-local-addr -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wextra -pedantic -Werror)
    #add_compile_options(-Wall -Wno-unused-variable -Wno-unused-parameter -Wextra -pedantic -Werror)
endif()

# If you are on 32 bit linux, uncomment this
# set (CMAKE_CXX_FLAGS "-m32")
# set (CMAKE_C_FLAGS "-m32")

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend")

# Adding Main Backend
if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/SFML")
elseif(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/Win32_OpenGL3_3")
elseif(ssGUI_MAIN_BACKEND STREQUAL "Template")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/Template")
elseif(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/X11_OpenGL3_3")
else()
    message(FATAL_ERROR "Invalid Backend")
endif()

# Adding Font Backend
if(ssGUI_FONT_BACKEND STREQUAL "FreeType")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/FreeType")
endif()

# Adding Image Backend
if(ssGUI_IMAGE_BACKEND STREQUAL "stb_image")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Backend/stb_image")
endif()

add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/DataClasses")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/EventCallbacks")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/Extensions")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/GUIObjectClasses")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/Src/ssGUI/GUIObjectClasses/CompositeClasses")

# ssGUI include directory
target_include_directories(ssGUI PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/Include"
                                        "${CMAKE_CURRENT_SOURCE_DIR}/External/glm"
                                        "${ssGUI_SFML_INCLUDE_DIR}")                            # This might not be needed

# DEBUG
# Get the include directories for the target.
# get_target_property(LIBA_INCLUDES ssGUI INCLUDE_DIRECTORIES)

# Construct the compiler string for the include directories.
# foreach(dir ${LIBA_INCLUDES} ${LIBB_INCLUDES})
#     string(APPEND INCLUDE_COMPILER_STRING "${dir}\n")
# endforeach()
# message("${INCLUDE_COMPILER_STRING}")

# Linking clip to ssGUI
target_link_libraries(ssGUI PUBLIC clip)

# Linking ssLogger to ssGUI
target_link_libraries(ssGUI PUBLIC ssLogger)

# ==========================================================
# Linking Backend to ssGUI and adding compiler definitions
# ==========================================================
if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
    target_link_libraries(ssGUI PUBLIC SFML::Network SFML::Audio SFML::Graphics SFML::Window SFML::System)
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_SFML)
elseif(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/glad_v0.1.36")
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_WIN32_OPENGL)
    target_include_directories(ssGUI PUBLIC "${CMAKE_CURRENT_LIST_DIR}/External/wglExt")
    find_package(OpenGL REQUIRED)
    target_link_libraries(ssGUI PUBLIC OpenGL::GL glad)
    set_target_properties(glad PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
elseif(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/glad_v0.1.36")
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_X11_OPENGL)
    find_package(OpenGL REQUIRED)
    find_package(X11 REQUIRED)
    target_link_libraries(ssGUI PUBLIC OpenGL::GL X11::X11 X11::Xrandr X11::Xcursor glad)
    set_target_properties(glad PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
elseif(ssGUI_MAIN_BACKEND STREQUAL "Template")
    message(WARNING "Template for ssGUI_MAIN_BACKEND should only be used for compiling, running it will result a crash")
else()
    message(FATAL_ERROR "Invalid Main Backend")
endif()

if(ssGUI_FONT_BACKEND STREQUAL "SFML")
    target_compile_definitions(ssGUI PUBLIC SSGUI_FONT_BACKEND_SFML)
elseif(ssGUI_FONT_BACKEND STREQUAL "FreeType")
    target_link_libraries(ssGUI PUBLIC freetype)
    target_compile_definitions(ssGUI PUBLIC SSGUI_FONT_BACKEND_FREE_TYPE)
elseif(ssGUI_FONT_BACKEND STREQUAL "Template")
    message(WARNING "Template for ssGUI_FONT_BACKEND should only be used for compiling, running it will result a crash")
else()
    message(FATAL_ERROR "Invalid Font Backend")
endif()

if(ssGUI_IMAGE_BACKEND STREQUAL "SFML")
    target_compile_definitions(ssGUI PUBLIC SSGUI_IMAGE_BACKEND_SFML)
elseif(ssGUI_IMAGE_BACKEND STREQUAL "stb_image")
    target_compile_definitions(ssGUI PUBLIC SSGUI_IMAGE_BACKEND_STB_IMAGE)
    target_link_libraries(ssGUI PUBLIC ssGUI_stb_image)                        # INTERFACE doesn't work for whatever reason
elseif(ssGUI_FONT_BACKEND STREQUAL "Template")
    message(WARNING "Template for ssGUI_FONT_BACKEND should only be used for compiling, running it will result a crash")
else()
    message(FATAL_ERROR "Invalid Image Backend")
endif()

# ==========================================================
# ssGUI compiler definitions
# ==========================================================
if(ssGUI_USE_DEBUG)
    target_compile_definitions(ssGUI PUBLIC SSGUI_USE_DEBUG=1)
endif()

if(ssGUI_DEBUG_STATE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_DEBUG_STATE=1)
endif()

if(ssGUI_REFRESH_CONSOLE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_REFRESH_CONSOLE=1)
endif()

if(ssGUI_SLOW_UPDATE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_SLOW_UPDATE=1)
endif()

if(ssGUI_USE_SFML_TIME)
    target_compile_definitions(ssGUI PUBLIC SSGUI_USE_SFML_TIME=1)
endif()

# ==========================================================
# Building tests and examples 
# ==========================================================

# Copy resources for the library 
file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
        DESTINATION "${CMAKE_BINARY_DIR}")

# ==========================================================
# Building tests and examples 
# ==========================================================

function(AddFilesToExecutables folderToExe binSubFolder ideFolderName)
    file(GLOB ssGUI_EXE_CPP_FILES "${folderToExe}*.cpp")

    foreach(CUR_SOURCE_PATH ${ssGUI_EXE_CPP_FILES})

        # Get the file name without path and extension
        get_filename_component(CUR_NAME "${CUR_SOURCE_PATH}" NAME_WE)  

        # Freetype test
        if(CUR_NAME STREQUAL "FreetypeTest" AND NOT ssGUI_FONT_BACKEND STREQUAL "FreeType")
            continue()
        elseif(CUR_NAME STREQUAL "FreetypeTest" AND ssGUI_FONT_BACKEND STREQUAL "FreeType")
            add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")
            target_link_libraries("${CUR_NAME}" PRIVATE freetype)
            continue()
        endif()

        # For now, skip custom graphics example if we don't have SFML backend
        if(CUR_NAME STREQUAL "CustomGraphicsExample" AND NOT ssGUI_MAIN_BACKEND STREQUAL "SFML")
            continue()
        endif()

        # add the executable
        add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")

        # Link ssGUI to executable
        target_link_libraries("${CUR_NAME}" PUBLIC ssGUI)

        # Sub-directory for build
        set_target_properties("${CUR_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binSubFolder})

        # Set folder name for IDEs such as Visual Studio
        set_target_properties("${CUR_NAME}" PROPERTIES FOLDER ${ideFolderName})

    endforeach(CUR_SOURCE_PATH "${ssGUI_EXE_CPP_FILES}")

endfunction()

# TODO: Leave it for now, but remove this later
#if(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
    AddFilesToExecutables("${CMAKE_CURRENT_SOURCE_DIR}/Src/BackendTests/" "${CMAKE_CURRENT_BINARY_DIR}/BackendTests/" "BackendTests")
#endif()


if(ssGUI_BUILD_TESTS)
    #AddFilesToExecutables("${CMAKE_CURRENT_SOURCE_DIR}/Src/Tests/" "${CMAKE_CURRENT_BINARY_DIR}/Tests/")
    
    AddFilesToExecutables("${CMAKE_CURRENT_SOURCE_DIR}/Src/Tests/AutoTests/BackendTests/" "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/BackendTests/" "AutoTests/BackendTests")

    # Copy resources for the Tests 
    file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Tests") 

endif()


if(ssGUI_BUILD_EXAMPLES)
    AddFilesToExecutables("${CMAKE_CURRENT_SOURCE_DIR}/Src/Examples/" "${CMAKE_CURRENT_BINARY_DIR}/Examples/" "Examples")

    # Copy resources for the Examples 
    file(   COPY "${CMAKE_CURRENT_SOURCE_DIR}/Resources"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Examples") 

endif()