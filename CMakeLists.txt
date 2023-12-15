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
set(ssGUI_LOG_LEVEL         "Warning"   CACHE STRING    
"Log level: 
None
Debug
Info
Warning
Error")

set(ssGUI_LOG_TAG           "None"      CACHE STRING    
"Log tag (overrides level): 
ssGUI_MANAGER_TAG
ssGUI_IMAGE_UTIL_TAG
ssGUI_BACKEND_TAG
ssGUI_DATA_TAG
ssGUI_EXT_TAG
ssGUI_GUI_OBJECT_TAG")

set(ssGUI_LOG_MANAGER_STATE     OFF CACHE BOOL "Log all the state in ssGUIManager")
set(ssGUI_USE_LOG_FUNC          OFF CACHE BOOL "Logs all recorded function call")
set(ssGUI_CAN_OVERRIDE_BACKEND  OFF CACHE BOOL "Ability to override backend functions, 
mostly for mocking")

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

set(ssGUI_MAIN_BACKEND_DESCRIPTION 
"The backend that can be used by ssGUI:
SDL-2
Mock
Template")

set(ssGUI_MAIN_BACKEND "SDL-2" CACHE STRING "${ssGUI_MAIN_BACKEND_DESCRIPTION}")

set_property(CACHE ssGUI_MAIN_BACKEND PROPERTY STRINGS  "SDL-2" 
                                                        "Mock" 
                                                        "Template")

if(ssGUI_MAIN_BACKEND STREQUAL "Mock")
    unset(ssGUI_CAN_OVERRIDE_BACKEND CACHE)
    set(ssGUI_CAN_OVERRIDE_BACKEND ON)
endif()

# Debug Variables
set(ssGUI_REFRESH_CONSOLE OFF CACHE BOOL "Clears the console at the end of each frame")
set(ssGUI_SLOW_UPDATE OFF CACHE BOOL "Fps will be set to 1 for easier debugging")

# Overriding ssLogger options
set(ssLOG_BUILD_TESTS OFF)
set(ssLOG_CALL_STACK OFF)
set(ssLOG_THREAD_SAFE OFF)

# Options for hiding submodules
set(ssGUI_HIDE_VARIABLES_ssLOGGER       OFF CACHE BOOL 
"If true, this will hide all ssLogger CMake variables")

set(ssGUI_HIDE_VARIABLES_CPP_OVERRIDE   OFF CACHE BOOL 
"If true, this will hide all CppOverride CMake variables")

set(ssGUI_HIDE_VARIABLES_CLIP           OFF CACHE BOOL 
"If true, this will hide all Clip CMake variables")

set(ssGUI_HIDE_VARIABLES_SDL2           OFF CACHE BOOL 
"If true, this will hide all SDL2 CMake variables")

set(ssGUI_HIDE_VARIABLES_FREETYPE       OFF CACHE BOOL 
"If true, this will hide all FreeType CMake variables")


set(ssGUI_EXPOSE_ssLOGGER       ON CACHE BOOL 
"If true, this will expose ssLogger CMake target")

set(ssGUI_EXPOSE_CPP_OVERRIDE   ON CACHE BOOL 
"If true, this will expose CppOverride CMake target")

set(ssGUI_EXPOSE_CLIP           ON CACHE BOOL 
"If true, this will expose Clip CMake target")

set(ssGUI_EXPOSE_FREETYPE       ON CACHE BOOL 
"If true, this will expose FreeType CMake target")

set(ssGUI_EXPOSE_GLAD           ON CACHE BOOL 
"If true, this will expose glad, opengl and window related CMake target")

set(ssGUI_EXPOSE_STB_IMAGE      ON CACHE BOOL 
"If true, this will expose stb image CMake target")


# ==========================================================
# ssGUI 
# ==========================================================
if(ssGUI_OUTPUT_STATIC)
    if(BUILD_SHARED_LIBS)
        message(FATAL_ERROR "ssGUI_OUTPUT_STATIC is ON but BUILD_SHARED_LIBS is also ON, 
                            please set BUILD_SHARED_LIBS to OFF")
    endif()

    # TODO(NOW): Uncomment this when finished testing backend
    add_library(ssGUI STATIC
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Factory.cpp"
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c"
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EventCallback.cpp"
)
else()
    #if(NOT BUILD_SHARED_LIBS)
    #    message(FATAL_ERROR "ssGUI_OUTPUT_STATIC is OFF but BUILD_SHARED_LIBS is also OFF, 
    #                        please set BUILD_SHARED_LIBS to ON")
    #endif()
    
    # TODO(NOW): Uncomment this when finished testing backend
    add_library(ssGUI SHARED
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/ssGUIManager.cpp"
    "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Factory.cpp"
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c"
    #"${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EventCallback.cpp"
)
endif()

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(ssGUI PRIVATE /utf-8 /Wall)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_compile_options(ssGUI PRIVATE    -Wall 
                                            -Wno-sign-compare 
                                            -Wno-unused-variable 
                                            -Wno-unused-parameter 
                                            -Wno-switch 
                                            -Wextra 
                                            -pedantic 
                                            -Werror)

# If you are on 32 bit linux, add -m32 to compile flags

endif()


add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend")

# Adding Main Backend
if(ssGUI_MAIN_BACKEND STREQUAL "SDL-2")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/SDL2")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/FreeType")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/stb_image")
    
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_SDL2=1)
    
#elseif(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
#    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Win32_OpenGL3_3")
#elseif(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
#    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/X11_OpenGL3_3")

elseif(ssGUI_MAIN_BACKEND STREQUAL "Template")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Template")
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_TEMPLATE=1)

elseif(ssGUI_MAIN_BACKEND STREQUAL "Mock")
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Backend/Mocks")
    target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_MOCK=1)

else()
    message(FATAL_ERROR "Invalid Backend")
endif()

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/DataClasses")

# TODO(NOW): Uncomment this when finished testing backend
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/Extensions")
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/GUIObjectClasses")
#add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/GUIObjectClasses/CompositeClasses")

# TODO(NOW): Add GLM as submodule instead

# ssGUI include directory
target_include_directories(ssGUI PUBLIC "${CMAKE_CURRENT_LIST_DIR}/Include"
                                        "${CMAKE_CURRENT_LIST_DIR}/External/glm")

if(NOT ssGUI_OUTPUT_STATIC)
    set_target_properties(ssGUI PROPERTIES INSTALL_RPATH "$ORIGIN")
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

if(ssGUI_USE_LOG_FUNC)
    target_compile_definitions(ssGUI PUBLIC ssGUI_USE_LOG_FUNC=1)
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

set(EMBED_EXEC_PATH "")
GET_EXEC_PATH(EMBED_EXEC_PATH)

function(CopyResources targetDir)
    file(MAKE_DIRECTORY ${targetDir})
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_LIST_DIR}/Resources ${targetDir}/Resources)
endfunction()

file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c" EMBEDDED_FILE_SIZE)
if(EMBEDDED_FILE_SIZE LESS 1024)
    set(DEFAULT_RESOURCES_TO_EMBED  "${CMAKE_CURRENT_LIST_DIR}/Resources/NotoSans-Regular.ttf"
                                    "NotoSans_Regular"
                                    "${CMAKE_CURRENT_LIST_DIR}/Resources/DownArrow.png"
                                    "DownArrow"
                                    "${CMAKE_CURRENT_LIST_DIR}/Resources/RotationCursor.png"
                                    "RotationCursor"
                                    "${CMAKE_CURRENT_LIST_DIR}/Resources/WindowIcon.png"
                                    "WindowIcon")
    
    #message("DEFAULT_RESOURCES_TO_EMBED: ${DEFAULT_RESOURCES_TO_EMBED}")
    
    EMBED_FILES("${EMBED_EXEC_PATH}" 
                "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c" 
                "${DEFAULT_RESOURCES_TO_EMBED}")

    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/Src/ssGUI/EmbeddedResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        message(WARNING "Failed to embed default resources")
    endif()
endif()

# ==========================================================
# Building tests and examples 
# ==========================================================

#CopyResources(${CMAKE_CURRENT_BINARY_DIR})

if(ssGUI_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/Tests")
endif()

# TODO(NOW): Uncomment this when finished testing backend
#if(ssGUI_BUILD_EXAMPLES)
#    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/Examples")
#endif()


# ==========================================================
# ssLogger 
# ==========================================================

if(ssGUI_OUTPUT_STATIC)
    set(ssLOG_OUTPUT_STATIC ON)
else()
    set(ssLOG_OUTPUT_STATIC OFF)
endif()

add_subdirectory(   "${CMAKE_CURRENT_LIST_DIR}/External/ssLogger" 
                    "${CMAKE_CURRENT_BINARY_DIR}/External/ssLogger")

# Set folder name for IDEs such as Visual Studio
set_target_properties(ssLogger PROPERTIES FOLDER External)

# TODO: Use ssLogger log level instead

if(ssGUI_HIDE_VARIABLES_ssLOGGER)
    mark_as_advanced(FORCE  ssLOG_OUTPUT_STATIC
                            ssLOG_USE_SOURCE
                            ssLOG_BUILD_TESTS
                            ssLOG_CALL_STACK
                            ssLOG_LOG_WITH_ASCII
                            ssLOG_SHOW_FILE_NAME
                            ssLOG_SHOW_LINE_NUM
                            ssLOG_SHOW_FUNC_NAME
                            ssLOG_SHOW_TIME
                            ssLOG_THREAD_SAFE
                            ssLOG_WRAP_WITH_BRACKET
                            ssLOG_LOG_TO_FILE
                            ssLOG_LEVEL)
else()
    mark_as_advanced(CLEAR  ssLOG_OUTPUT_STATIC
                            ssLOG_USE_SOURCE
                            ssLOG_BUILD_TESTS
                            ssLOG_CALL_STACK
                            ssLOG_LOG_WITH_ASCII
                            ssLOG_SHOW_FILE_NAME
                            ssLOG_SHOW_LINE_NUM
                            ssLOG_SHOW_FUNC_NAME
                            ssLOG_SHOW_TIME
                            ssLOG_THREAD_SAFE
                            ssLOG_WRAP_WITH_BRACKET
                            ssLOG_LOG_TO_FILE
                            ssLOG_LEVEL)
endif()

if(ssGUI_EXPOSE_ssLOGGER)
    target_link_libraries(ssGUI PUBLIC ssLogger)
else()
    target_link_libraries(ssGUI PRIVATE ssLogger)
endif()



# ==========================================================
# ssTest 
# ==========================================================

set(ssTEST_BUILD_CHECK OFF)
add_subdirectory(   "${CMAKE_CURRENT_LIST_DIR}/External/ssTest" 
                    "${CMAKE_CURRENT_BINARY_DIR}/External/ssTest")

# Set folder name for IDEs such as Visual Studio
set_target_properties(ssTest PROPERTIES FOLDER External)

if(ssTEST_EXPOSE)
    target_link_libraries(ssGUI PUBLIC ssTest)
else()
    target_link_libraries(ssGUI PRIVATE ssTest)
endif()


# ==========================================================
# CppOverride
# ==========================================================

if(ssGUI_CAN_OVERRIDE_BACKEND)
    add_subdirectory(   "${CMAKE_CURRENT_LIST_DIR}/External/CppOverride" 
                        "${CMAKE_CURRENT_BINARY_DIR}/External/CppOverride")
    
    # Set folder name for IDEs such as Visual Studio
    set_target_properties(CppOverride PROPERTIES FOLDER External)

    if(ssGUI_HIDE_VARIABLES_CPP_OVERRIDE)
        mark_as_advanced(FORCE  CPP_OVERRIDE_BUILD_TEST
                                CPP_OVERRIDE_BUILD_EXAMPLE)
    else()
        mark_as_advanced(CLEAR  CPP_OVERRIDE_BUILD_TEST
                                CPP_OVERRIDE_BUILD_EXAMPLE)
    endif()
    
    if(ssGUI_EXPOSE_CPP_OVERRIDE)
        target_link_libraries(ssGUI PUBLIC CppOverride)
    else()
        target_link_libraries(ssGUI PRIVATE CppOverride)
    endif()
    
    target_compile_definitions(ssGUI PUBLIC SSGUI_OVERRIDE_BACKEND=1)
endif()

if(ssGUI_MAIN_BACKEND STREQUAL "SDL-2")
    
    ## ==========================================================
    ## clip 
    ## ==========================================================
    ## Disable clip examples and tests
    #set(CLIP_EXAMPLES OFF CACHE BOOL "Compile clip examples")
    #set(CLIP_TESTS OFF CACHE BOOL "Compile clip tests")

    ## In case that you have ${PNG_LIBRARY} set to support copy/paste images on Linux
    ##set(CLIP_X11_PNG_LIBRARY "${PNG_LIBRARY}")

    ## Add clip subdirectory to compile the library
    #add_subdirectory("${PROJECT_SOURCE_DIR}/External/clip")
    
    #if(ssGUI_HIDE_VARIABLES_CLIP)
    #    mark_as_advanced(FORCE  CLIP_EXAMPLES
    #                            CLIP_TESTS
    #                            CLIP_X11_WITH_PNG
    #                            PNG_LIBRARY)
    #else()
    #    mark_as_advanced(CLEAR  CLIP_EXAMPLES
    #                            CLIP_TESTS
    #                            CLIP_X11_WITH_PNG
    #                            PNG_LIBRARY)
    #endif()

    #target_include_directories(clip PUBLIC "${PROJECT_SOURCE_DIR}/External/clip")

    ## Set folder name for IDEs such as Visual Studio
    #set_target_properties(clip PROPERTIES FOLDER External)

    # ==========================================================
    # FreeType
    # ==========================================================
    
    set(ORIGINAL_BUILD_SHARED_LIBS_VALUE ${BUILD_SHARED_LIBS})
    
    unset(BUILD_SHARED_LIBS CACHE)
    if(ssGUI_OUTPUT_STATIC)
        set(BUILD_SHARED_LIBS OFF)
    else()
        set(BUILD_SHARED_LIBS ON)
    endif()
    
    add_subdirectory(   "${CMAKE_CURRENT_LIST_DIR}/External/freetype" 
                        "${CMAKE_CURRENT_BINARY_DIR}/External/freetype")
    
    set(BUILD_SHARED_LIBS ${ORIGINAL_BUILD_SHARED_LIBS_VALUE} CACHE BOOL "" FORCE)
    
    # Disabling FT_DISABLE_HARFBUZZ by default as it is causing errors
    unset(FT_DISABLE_HARFBUZZ CACHE)
    set(FT_DISABLE_HARFBUZZ ON)
    
    # Set folder name for IDEs such as Visual Studio
    set_target_properties(freetype PROPERTIES FOLDER External)
    
    
    if(ssGUI_HIDE_VARIABLES_FREETYPE)
        mark_as_advanced(FORCE  FT_DISABLE_ZLIB
                                FT_DISABLE_BZIP2
                                FT_DISABLE_PNG
                                FT_DISABLE_HARFBUZZ
                                FT_DISABLE_BROTLI
                                FT_ENABLE_ERROR_STRINGS
                                FT_REQUIRE_ZLIB
                                FT_REQUIRE_BZIP2
                                FT_REQUIRE_PNG
                                FT_REQUIRE_HARFBUZZ
                                FT_REQUIRE_BROTLI)
    else()
        mark_as_advanced(CLEAR  FT_DISABLE_ZLIB
                                FT_DISABLE_BZIP2
                                FT_DISABLE_PNG
                                FT_DISABLE_HARFBUZZ
                                FT_DISABLE_BROTLI
                                FT_ENABLE_ERROR_STRINGS
                                FT_REQUIRE_ZLIB
                                FT_REQUIRE_BZIP2
                                FT_REQUIRE_PNG
                                FT_REQUIRE_HARFBUZZ
                                FT_REQUIRE_BROTLI)
    endif()

    if(ssGUI_EXPOSE_FREETYPE)
        target_link_libraries(ssGUI PUBLIC freetype)
    else()
        target_link_libraries(ssGUI PRIVATE freetype)
    endif()


    # ==========================================================
    # stb_image
    # ==========================================================
    add_library(ssGUI_stb_image INTERFACE)
    
    #target_sources( stb_image INTERFACE 
    #                FILE_SET HEADERS
    #                BASE_DIRS "${CMAKE_CURRENT_LIST_DIR}/External/stb_image"
    #                FILES "${CMAKE_CURRENT_LIST_DIR}/External/stb_image/stb_image.h")
    target_include_directories(ssGUI_stb_image INTERFACE "${CMAKE_CURRENT_LIST_DIR}/External/stb_image")
    
    # Set folder name for IDEs such as Visual Studio
    set_target_properties(ssGUI_stb_image PROPERTIES FOLDER External)

    if(ssGUI_EXPOSE_stb_image)
        target_link_libraries(ssGUI PUBLIC ssGUI_stb_image)
    else()
        target_link_libraries(ssGUI PRIVATE ssGUI_stb_image)
    endif()
    
    
    # ==========================================================
    # SDL2
    # ==========================================================
    if(ssGUI_OUTPUT_STATIC)
        set(SDL_STATIC ON)
        set(SDL_SHARED OFF)
    else()
        set(SDL_SHARED ON)
        set(SDL_STATIC OFF)
    endif()

    add_subdirectory(   "${CMAKE_CURRENT_LIST_DIR}/External/SDL" 
                        "${CMAKE_CURRENT_BINARY_DIR}/External/SDL")

    if(ssGUI_OUTPUT_STATIC)
        # Set folder name for IDEs such as Visual Studio
        set_target_properties(SDL2-static PROPERTIES FOLDER External)
        
        if(ssGUI_EXPOSE_SDL2)
            target_link_libraries(ssGUI PUBLIC SDL2-static)
        else()
            target_link_libraries(ssGUI PRIVATE SDL2-static)
        endif()
    else()
        # Set folder name for IDEs such as Visual Studio
        set_target_properties(SDL2 PROPERTIES FOLDER External)
        
        if(ssGUI_EXPOSE_SDL2)
            target_link_libraries(ssGUI PUBLIC SDL2)
        else()
            target_link_libraries(ssGUI PRIVATE SDL2)
        endif()
    endif()
    
    
endif()

