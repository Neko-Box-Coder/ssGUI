cmake_minimum_required(VERSION 3.14)
# cmake_minimum_required(VERSION 3.24)

if(TARGET ssGUI)
    return()
endif()

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
set(ssGUI_OUTPUT_STATIC     ON          CACHE BOOL      "Use and create static library instead")
set(ssGUI_BUILD_TESTS       OFF         CACHE BOOL      "Build tests")
set(ssGUI_BUILD_EXAMPLES    ON          CACHE BOOL      "Build examples")
set(ssGUI_LOG_LEVEL         "Warning"   CACHE STRING    "Log level")
set(ssGUI_LOG_TAG           "None"      CACHE STRING    "Log tag (overrides level)")
set(ssGUI_LOG_MANAGER_STATE OFF         CACHE BOOL      "Log all the state in ssGUIManager")

#Dropdown
set_property(CACHE ssGUI_LOG_LEVEL PROPERTY STRINGS "None" 
                                                    "Debug" 
                                                    "Info" 
                                                    "Warning" 
                                                    "Error")
                                                    
set_property(CACHE ssGUI_LOG_TAG PROPERTY STRINGS   "None" 
                                                    "ssGUI_MANAGER_TAG" 
                                                    "ssGUI_IMAGE_UTIL_TAG" 
                                                    "ssGUI_BACKEND_TAG" 
                                                    "ssGUI_DATA_TAG" 
                                                    "ssGUI_EXT_TAG" 
                                                    "ssGUI_GUI_OBJECT_TAG")

set(ssGUI_MAIN_BACKEND_DESCRIPTION "The main backend that will be used by ssGUI")
set(ssGUI_FONT_BACKEND_DESCRIPTION "The font backend that will be used by ssGUI")
set(ssGUI_IMAGE_BACKEND_DESCRIPTION "The image backend that will be used by ssGUI")


# Default backend options variable
if(NOT ssGUI_MAIN_BACKEND STREQUAL "SFML" AND NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
    if(WIN32)
        unset(ssGUI_MAIN_BACKEND CACHE)
        set(ssGUI_MAIN_BACKEND "Win32_OpenGL3_3" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")
        set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS  "Win32_OpenGL3_3" 
                                                                "SFML" 
                                                                "Template")
    # Linux
    else()
        unset(ssGUI_MAIN_BACKEND CACHE)
        set(ssGUI_MAIN_BACKEND "X11_OpenGL3_3" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")
        set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS  "X11_OpenGL3_3" 
                                                                "SFML" 
                                                                "Template")
    endif()
endif()

# Use Mock Backend
set(ssGUI_MOCK_OFF_OPTION_STRING "Off")
set(ssGUI_MOCK_OPTION_STRING "Mock")
set(ssGUI_MOCK_WITH_NATIVE_OPTION_STRING "Mock With Native Backend")

set(ssGUI_MOCK_BACKEND_MODE "Off" CACHE STRING "Mock backend mode")
set_property(CACHE ssGUI_MOCK_BACKEND_MODE PROPERTY STRINGS "${ssGUI_MOCK_OFF_OPTION_STRING}" 
                                                            "${ssGUI_MOCK_OPTION_STRING}" 
                                                            "${ssGUI_MOCK_WITH_NATIVE_OPTION_STRING}")

if(ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
    unset(ssGUI_MAIN_BACKEND CACHE)
    unset(ssGUI_FONT_BACKEND CACHE)
    unset(ssGUI_IMAGE_BACKEND CACHE)
    set(ssGUI_MAIN_BACKEND "Mock" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")
    set(ssGUI_FONT_BACKEND "Mock" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
    set(ssGUI_IMAGE_BACKEND "Mock" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")
    set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS  "Mock") 
    set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS  "Mock") 
    set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "Mock")
    unset(ssGUI_USE_LOCAL_SFML CACHE)
    unset(ssGUI_USE_SFML_TIME CACHE)
else()
    # Show / unshow SFML options depending if SFML backend is selected or not
    if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
        set(ssGUI_USE_LOCAL_SFML ON CACHE BOOL "Use the SFML that comes with ssGUI")
        set(ssGUI_USE_SFML_TIME OFF CACHE BOOL "Use SFML time instead of chrono time")
        
        unset(ssGUI_FONT_BACKEND CACHE)
        unset(ssGUI_IMAGE_BACKEND CACHE)
        set(ssGUI_FONT_BACKEND "SFML" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
        set(ssGUI_IMAGE_BACKEND "SFML" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")
        set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS  "SFML" 
                                                                "FreeType" 
                                                                "Template")
        
        set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "SFML" 
                                                                "stb_image" 
                                                                "Template")

        # This might not be needed 
        set(ssGUI_SFML_INCLUDE_DIR "${CMAKE_CURRENT_LIST_DIR}/External/sfml/include" CACHE PATH "Provide the directory of ")
        set (CMAKE_CXX_STANDARD 17) #SFML needs C++17
    else()
        unset(ssGUI_USE_LOCAL_SFML CACHE)
        unset(ssGUI_USE_SFML_TIME CACHE)
    endif()

    if(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
        unset(ssGUI_FONT_BACKEND CACHE)
        unset(ssGUI_IMAGE_BACKEND CACHE)
        set(ssGUI_FONT_BACKEND "FreeType" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
        set(ssGUI_IMAGE_BACKEND "stb_image" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

        set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS  "FreeType" 
                                                                "Template")
        
        set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "stb_image" 
                                                                "Template")
    endif()

    if(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
        unset(ssGUI_FONT_BACKEND CACHE)
        unset(ssGUI_IMAGE_BACKEND CACHE)
        set(ssGUI_FONT_BACKEND "FreeType" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
        set(ssGUI_IMAGE_BACKEND "stb_image" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

        set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS  "FreeType" 
                                                                "Template")
        
        set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "stb_image" 
                                                                "Template")
    endif()

    if(ssGUI_MAIN_BACKEND STREQUAL "Template")
        unset(ssGUI_FONT_BACKEND CACHE)
        unset(ssGUI_IMAGE_BACKEND CACHE)
        set(ssGUI_FONT_BACKEND "Template" CACHE STRING "${ssGUI_FONT_BACKEND_DESCRIPTION}")
        set(ssGUI_IMAGE_BACKEND "Template" CACHE STRING "${ssGUI_IMAGE_BACKEND_DESCRIPTION}")

        set_property(CACHE ssGUI_FONT_BACKEND PROPERTY STRINGS "Template")
        set_property(CACHE ssGUI_IMAGE_BACKEND PROPERTY STRINGS "Template")
    endif()
endif()

# Debug Variables
set(ssGUI_REFRESH_CONSOLE OFF CACHE BOOL "Clears the console at the end of each frame")
set(ssGUI_SLOW_UPDATE OFF CACHE BOOL "Fps will be set to 1 for easier debugging")

# Overriding ssLogger options
set(ssLOG_BUILD_TESTS   "FALSE" CACHE BOOL "If true, this will build tests")
set(ssLOG_CALL_STACK    "FALSE" CACHE BOOL "If true, this will build tests")
set(ssLOG_THREAD_SAFE   "FALSE" CACHE BOOL "Use std::thread and ensure thread safety for all logged functions")

# Dividers
set(ssGUI@_CMAKE_VARIABLES_START============================= "===========================================================" CACHE STRING "Divider") 
set(ssGUI__CMAKE_VARIABLES_END=============================== "===========================================================" CACHE STRING "Divider") 

#Find shared libraries in the same location as the executable
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")

# target_include_directories(example PUBLIC "${INCLUDE_LOCATION}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++0x")  #For surpressing goto warning

# ==========================================================
# ssLogger 
# ==========================================================

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/ssLogger" "${CMAKE_CURRENT_BINARY_DIR}/External/ssLogger")

set_target_properties(ssLogger PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio

# ==========================================================
# ssTest 
# ==========================================================

set(ssTEST_BUILD_CHECK OFF)
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/ssTest" "${CMAKE_CURRENT_BINARY_DIR}/External/ssTest")
set_target_properties(ssTest PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio


# ==========================================================
# SimpleOverride
# ==========================================================

if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OFF_OPTION_STRING}")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/SimpleOverride" "${CMAKE_CURRENT_BINARY_DIR}/External/SimpleOverride")
    set_target_properties(SimpleOverride PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
endif()

if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
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

        set(SFML_DIR "${CMAKE_CURRENT_LIST_DIR}/External/sfml/lib/cmake/SFML")

        # set(INCLUDE_LOCATION "${CMAKE_CURRENT_LIST_DIR}/../include")
        # message("${SFML_DIR}")

        # target_link_options(ssGUI PRIVATE -mwindows) # uncomment to disabling console on Windows
        if(ssGUI_OUTPUT_STATIC)
            set(SFML_STATIC_LIBRARIES TRUE)
        endif()

        # Import SFML
        find_package(SFML 3 COMPONENTS System Window Graphics Network Audio REQUIRED)

        # TODO: Check if this works
        set_target_properties(SFML::System PROPERTIES FOLDER External)      # Set folder name for IDEs such as Visual Studio
        set_target_properties(SFML::Window PROPERTIES FOLDER External)      # Set folder name for IDEs such as Visual Studio
        set_target_properties(SFML::Graphics PROPERTIES FOLDER External)    # Set folder name for IDEs such as Visual Studio
        set_target_properties(SFML::Network PROPERTIES FOLDER External)     # Set folder name for IDEs such as Visual Studio
        set_target_properties(SFML::Audio PROPERTIES FOLDER External)       # Set folder name for IDEs such as Visual Studio
    endif()

    # ==========================================================
    # FreeType
    # ==========================================================
    if(ssGUI_FONT_BACKEND STREQUAL "FreeType")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/freetype" "${CMAKE_CURRENT_BINARY_DIR}/External/freetype")
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
        #                BASE_DIRS "${CMAKE_CURRENT_LIST_DIR}/External/stb_image"
        #                FILES "${CMAKE_CURRENT_LIST_DIR}/External/stb_image/stb_image.h")
        target_include_directories(ssGUI_stb_image INTERFACE "${CMAKE_CURRENT_LIST_DIR}/External/stb_image")
        set_target_properties(ssGUI_stb_image PROPERTIES FOLDER External)   # Set folder name for IDEs such as Visual Studio
    endif()
endif()

# ==========================================================
# ssGUI 
# ==========================================================
if(ssGUI_OUTPUT_STATIC)
    add_library(ssGUI STATIC
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Factory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EventCallback.cpp"
)
else()
    add_library(ssGUI SHARED
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Factory.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EventCallback.cpp"
)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    add_compile_options(/utf-8)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ssGUI PRIVATE -Wall -Wno-return-local-addr -Wno-sign-compare -Wno-unused-variable -Wno-unused-parameter -Wno-switch -Wextra -pedantic -Werror)
    #add_compile_options(-Wall -Wno-unused-variable -Wno-unused-parameter -Wextra -pedantic -Werror)
endif()

# If you are on 32 bit linux, uncomment this
# set (CMAKE_CXX_FLAGS "-m32")
# set (CMAKE_C_FLAGS "-m32")

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend")

if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
    # Adding Main Backend
    if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/SFML")
    elseif(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Win32_OpenGL3_3")
    elseif(ssGUI_MAIN_BACKEND STREQUAL "Template")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Template")
    elseif(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/X11_OpenGL3_3")
    else()
        message(FATAL_ERROR "Invalid Backend")
    endif()

    # Adding Font Backend
    if(ssGUI_FONT_BACKEND STREQUAL "FreeType")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/FreeType")
    endif()

    # Adding Image Backend
    if(ssGUI_IMAGE_BACKEND STREQUAL "stb_image")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/stb_image")
    endif()
endif()

# Add Mock Backend
if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OFF_OPTION_STRING}")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Mocks")
endif()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/DataClasses")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Extensions")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/GUIObjectClasses")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/GUIObjectClasses/CompositeClasses")

# ssGUI include directory
target_include_directories(ssGUI PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Include"
                                        "${CMAKE_CURRENT_LIST_DIR}/External/glm"
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
if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
    target_link_libraries(ssGUI PUBLIC clip)
endif()

# Linking ssLogger to ssGUI
target_link_libraries(ssGUI PUBLIC ssLogger)

target_link_libraries(ssGUI PUBLIC ssTest)

# ==========================================================
# Linking Backend to ssGUI and adding backend compiler definitions
# ==========================================================
if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
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
endif()

if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OFF_OPTION_STRING}")
    target_link_libraries(ssGUI PUBLIC SimpleOverride)
    target_compile_definitions(ssGUI PUBLIC SSGUI_MOCK_BACKEND)
    
    if(ssGUI_MAIN_BACKEND STREQUAL "Mock")
        target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_MOCK)
    endif()
    
    if(ssGUI_FONT_BACKEND STREQUAL "Mock")
        target_compile_definitions(ssGUI PUBLIC SSGUI_FONT_BACKEND_MOCK)
    endif()
    
    if(ssGUI_IMAGE_BACKEND STREQUAL "Mock")
        target_compile_definitions(ssGUI PUBLIC SSGUI_IMAGE_BACKEND_MOCK)
    endif()
    
endif()

# ==========================================================
# ssGUI compiler definitions
# ==========================================================
if(ssGUI_REFRESH_CONSOLE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_REFRESH_CONSOLE=1)
endif()

if(ssGUI_SLOW_UPDATE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_SLOW_UPDATE=1)
endif()

if(ssGUI_USE_SFML_TIME)
    target_compile_definitions(ssGUI PUBLIC SSGUI_USE_SFML_TIME=1)
endif()

if(ssGUI_LOG_MANAGER_STATE)
    target_compile_definitions(ssGUI PUBLIC SSGUI_LOG_MANAGER_STATE=1)
endif()

#See ssGUI/HelperClasses/LogWithTagsAndLevel.hpp for log tags and levels
if(NOT ssGUI_LOG_TAG STREQUAL "None")
    if(ssGUI_LOG_TAG STREQUAL "ssGUI_MANAGER_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=1)
    elseif(ssGUI_LOG_TAG STREQUAL "ssGUI_IMAGE_UTIL_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=2)
    elseif(ssGUI_LOG_TAG STREQUAL "ssGUI_BACKEND_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=3)
    elseif(ssGUI_LOG_TAG STREQUAL "ssGUI_DATA_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=4)
    elseif(ssGUI_LOG_TAG STREQUAL "ssGUI_EXT_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=5)
    elseif(ssGUI_LOG_TAG STREQUAL "ssGUI_GUI_OBJECT_TAG")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_TAG=6)
    endif()
else()

    if(ssGUI_LOG_LEVEL STREQUAL "Debug")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_LEVEL=4)
    elseif(ssGUI_LOG_LEVEL STREQUAL "Info")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_LEVEL=3)
    elseif(ssGUI_LOG_LEVEL STREQUAL "Warning")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_LEVEL=2)
    elseif(ssGUI_LOG_LEVEL STREQUAL "Error")
        target_compile_definitions(ssGUI PUBLIC ssGUI_LOG_LEVEL=1)
    endif()

endif()

# ==========================================================
# Copy and embed resources 
# ==========================================================

# This is only called when embed target is specified for CMake
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/Embed2C" EXCLUDE_FROM_ALL)
include("${CMAKE_CURRENT_LIST_DIR}/External/Embed2C/embedFile.cmake")

function(CopyResources targetDir)
    file(MAKE_DIRECTORY ${targetDir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/Resources ${targetDir}/Resources)
endfunction()

file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c" EMBEDDED_FILE_SIZE)
if(EMBEDDED_FILE_SIZE LESS 1024)
    EMBED_FILES("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c"
                "${CMAKE_CURRENT_LIST_DIR}/Resources/NotoSans-Regular.ttf"
                "NotoSans_Regular"
                "${CMAKE_CURRENT_LIST_DIR}/Resources/DownArrow.png"
                "DownArrow"
                "${CMAKE_CURRENT_LIST_DIR}/Resources/RotationCursor.png"
                "RotationCursor"
                "${CMAKE_CURRENT_LIST_DIR}/Resources/WindowIcon.png"
                "WindowIcon")

    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        message(WARNING "Failed to embed default resources")
    endif()
endif()

# ==========================================================
# Building tests and examples 
# ==========================================================

function(AddFilesToExecutables folderToExe binSubFolder ideFolderName targetsToLink)
    file(GLOB ssGUI_EXE_CPP_FILES "${folderToExe}*.cpp")

    foreach(CUR_SOURCE_PATH ${ssGUI_EXE_CPP_FILES})

        # Get the file name without path and extension
        get_filename_component(CUR_NAME "${CUR_SOURCE_PATH}" NAME_WE)  

        # Freetype test
        #if(CUR_NAME STREQUAL "FreetypeTest" AND NOT ssGUI_FONT_BACKEND STREQUAL "FreeType")
        #    continue()
        #elseif(CUR_NAME STREQUAL "FreetypeTest" AND ssGUI_FONT_BACKEND STREQUAL "FreeType")
        #    add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")
        #    target_link_libraries("${CUR_NAME}" PRIVATE freetype)
        #    continue()
        #endif()

        # For now, skip custom graphics example if we are using SFML backend
        STRING(TOLOWER ${ssGUI_MAIN_BACKEND} ssGUI_LOWER_MAIN_BACKEND)
        if((CUR_NAME STREQUAL "CustomGraphicsExampleV2" OR CUR_NAME STREQUAL "DynamicAtlasTest_Auto") AND NOT ssGUI_LOWER_MAIN_BACKEND MATCHES ".*opengl.*")
            continue()
        endif()

        # add the executable
        add_executable("${CUR_NAME}" "${CUR_SOURCE_PATH}")

        # Link targets
        foreach(linkTarget ${targetsToLink})
            target_link_libraries("${CUR_NAME}" PUBLIC ${linkTarget})
        endforeach()

        # Sub-directory for build
        set_target_properties("${CUR_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binSubFolder})

        # Set folder name for IDEs such as Visual Studio
        set_target_properties("${CUR_NAME}" PROPERTIES FOLDER ${ideFolderName})

    endforeach(CUR_SOURCE_PATH "${ssGUI_EXE_CPP_FILES}")

    #CopyResources(${binSubFolder})

endfunction()

#CopyResources(${CMAKE_CURRENT_BINARY_DIR})


if(ssGUI_BUILD_TESTS)
    # Check if test resources is populated
    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/TestsResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        EMBED_FILES("${CMAKE_CURRENT_LIST_DIR}/Src/Tests/TestsResources.c"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/arial.ttf"
                    "ssGUI_Test_arial"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/NotoColorEmoji.ttf"
                    "ssGUI_Test_NotoColorEmoji"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/sd.png"
                    "ssGUI_Test_sd"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/sdTestEdge.png"
                    "ssGUI_Test_sd_edge"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/NotoSans-Bold.ttf"
                    "ssGUI_Test_NotoSans_Bold")

        file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/TestsResources.c" EMBEDDED_FILE_SIZE)
        if(EMBEDDED_FILE_SIZE LESS 1024)
            message(WARNING "Failed to embed test resources")
        endif()
    endif()
    
    add_library(ssGUI_Test_Resources "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/TestsResources.c")
    target_include_directories(ssGUI_Test_Resources PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Src/Tests")
    set_target_properties(ssGUI_Test_Resources PROPERTIES FOLDER "Tests")

    # ----------------------------------------------------------
    # Auto Tests
    # ----------------------------------------------------------
    #if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
        AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/BackendTests/" 
                                "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/BackendTests/" 
                                "Tests/AutoTests/BackendTests"
                                "ssGUI;ssGUI_Test_Resources")
    #else()
    #    message(WARNING "ssGUI_MOCK_BACKEND_MODE is set to ${ssGUI_MOCK_OPTION_STRING}, skipping backend tests")
    #    message(WARNING "Set ssGUI_MOCK_BACKEND_MODE to not be ${ssGUI_MOCK_OPTION_STRING} in order to produce backend tests")
    #endif()
    
    
    if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OFF_OPTION_STRING}")
        AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/EventCallbackTests/" 
                                "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/EventCallbackTests/" 
                                "Tests/AutoTests/EventCallbackTests"
                                "ssGUI;ssGUI_Test_Resources")
        
        AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/GUIObjectTests/" 
                                "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/GUIObjectTests/" 
                                "Tests/AutoTests/GUIObjectTests"
                                "ssGUI;ssGUI_Test_Resources")    
    else()
        message(WARNING "ssGUI_MOCK_BACKEND_MODE is set to ${ssGUI_MOCK_OFF_OPTION_STRING}, skipping tests")
    endif()
    
    if(WIN32)
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/")
        file(   COPY "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/RunAllTests.bat"
                DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests")
        #execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/RunAllTests.bat" "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/RunAllTests.bat")
    else()
        file(   CHMOD "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/RunAllTests.sh"
                PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE)
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/")
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/RunAllTests.sh" "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/RunAllTests.sh")
    endif()

    # ----------------------------------------------------------
    # Manual Tests
    # ----------------------------------------------------------
    if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
        AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/ManualTests/BackendTests/" 
                                "${CMAKE_CURRENT_BINARY_DIR}/Tests/ManualTests/BackendTests/" 
                                "Tests/ManualTests/BackendTests"
                                "ssGUI;ssGUI_Test_Resources")
    else()
        message(WARNING "ssGUI_MOCK_BACKEND_MODE is set to ${ssGUI_MOCK_OPTION_STRING}, skipping manual backend tests")
        message(WARNING "Set ssGUI_MOCK_BACKEND_MODE to not be ${ssGUI_MOCK_OPTION_STRING} in order to produce manual backend tests")
    endif()

    # TODO: The aim is to replace these manual tests with auto tests, unless it is backend tests I guess
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/ManualTests/EventCallbackTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Tests/ManualTests/EventCallbackTests/" 
                            "Tests/ManualTests/EventCallbackTests"
                            "ssGUI;ssGUI_Test_Resources")
    
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/ManualTests/GUIObjectTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Tests/ManualTests/GUIObjectTests/" 
                            "Tests/ManualTests/GUIObjectTests"
                            "ssGUI;ssGUI_Test_Resources")
    
    # ----------------------------------------------------------
    # Tests Templates
    # ----------------------------------------------------------
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/AutoTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Tests/AutoTests/" 
                            "Tests/AutoTests"
                            "ssGUI;ssGUI_Test_Resources")
    
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Tests/ManualTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Tests/ManualTests/" 
                            "Tests/ManualTests"
                            "ssGUI;ssGUI_Test_Resources")
endif()

if(ssGUI_BUILD_EXAMPLES)
    # Check if example resources is populated
    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/Examples/ExamplesResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        EMBED_FILES("${CMAKE_CURRENT_LIST_DIR}/Src/Examples/ExamplesResources.c"                    
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/sd.png"
                    "ssGUI_Example_sd"
                    "${CMAKE_CURRENT_LIST_DIR}/Resources/NotoSans-Bold.ttf"
                    "ssGUI_Example_NotoSans_Bold")

        file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/Examples/ExamplesResources.c" EMBEDDED_FILE_SIZE)
        if(EMBEDDED_FILE_SIZE LESS 1024)
            message(WARNING "Failed to embed example resources")
        endif()
    endif()
    
    add_library(ssGUI_Example_Resources "${CMAKE_CURRENT_LIST_DIR}/Src/Examples/ExamplesResources.c")
    target_include_directories(ssGUI_Example_Resources PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Src/Examples")
    set_target_properties(ssGUI_Example_Resources PROPERTIES FOLDER "Examples")

    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Examples/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Examples/" 
                            "Examples"
                            "ssGUI;ssGUI_Example_Resources")
    
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Src/Examples/Playground/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/Examples/Playground/" 
                            "Examples/Playground"
                            "ssGUI;ssGUI_Example_Resources")
endif()