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


set(ssGUI_LOG_MANAGER_STATE OFF         CACHE BOOL      "Log all the state in ssGUIManager")
set(ssGUI_USE_LOG_FUNC      OFF         CACHE BOOL      "Logs all recorded function call")

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
"The main backend that will be used by ssGUI:
Win32_OpenGL3_3
X11_OpenGL3_3
SFML
Mock
Template")


set(ssGUI_FONT_BACKEND_DESCRIPTION 
"The font backend that will be used by ssGUI:
FreeType
SFML
Mock
Template")


set(ssGUI_IMAGE_BACKEND_DESCRIPTION 
"The image backend that will be used by ssGUI:
stb_image
SFML
Mock
Template")

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

set(ssGUI_MOCK_BACKEND_MODE "${ssGUI_MOCK_OFF_OPTION_STRING}" CACHE STRING 
"Mock backend mode:
${ssGUI_MOCK_OFF_OPTION_STRING}
${ssGUI_MOCK_OPTION_STRING}
${ssGUI_MOCK_WITH_NATIVE_OPTION_STRING}")

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

# Options for hiding submodules
set(ssGUI_HIDE_VARIABLES_ssLOGGER ON CACHE BOOL "If true, this will hide all ssLogger CMake variables")
set(ssGUI_HIDE_VARIABLES_SIMPLE_OVERRIDE ON CACHE BOOL "If true, this will hide all SimpleOverride CMake variables")
set(ssGUI_HIDE_VARIABLES_CLIP ON CACHE BOOL "If true, this will hide all Clip CMake variables")
set(ssGUI_HIDE_VARIABLES_FREETYPE ON CACHE BOOL "If true, this will hide all FreeType CMake variables")
#TODO: Add SFML

set(ssGUI_HIDE_TARGET_ssLOGGER ON CACHE BOOL "If true, this will hide ssLogger CMake target")
set(ssGUI_HIDE_TARGET_SIMPLE_OVERRIDE ON CACHE BOOL "If true, this will hide SimpleOverride CMake target")
set(ssGUI_HIDE_TARGET_CLIP ON CACHE BOOL "If true, this will hide Clip CMake target")
set(ssGUI_HIDE_TARGET_FREETYPE ON CACHE BOOL "If true, this will hide FreeType CMake target")
set(ssGUI_HIDE_TARGET_SFML ON CACHE BOOL "If true, this will hide sfml CMake target")
set(ssGUI_HIDE_TARGET_GLAD ON CACHE BOOL "If true, this will hide glad, opengl and window related CMake target")
set(ssGUI_HIDE_TARGET_STB_IMAGE ON CACHE BOOL "If true, this will hide stb image CMake target")


# Dividers
set(ssGUI_@CMAKE_VARIABLES_START============================= "===========================================================" CACHE STRING "Divider") 
set(ssGUI__CMAKE_VARIABLES_END=============================== "===========================================================" CACHE STRING "Divider") 

if (NOT CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    #Find shared libraries in the same location as the executable
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,$ORIGIN")

    # target_include_directories(example PUBLIC "${INCLUDE_LOCATION}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive -std=c++0x")  #For surpressing goto warning
endif()

# ==========================================================
# ssLogger 
# ==========================================================

add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/ssLogger" "${CMAKE_CURRENT_BINARY_DIR}/External/ssLogger")
set_target_properties(ssLogger PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio

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

if(ssGUI_HIDE_VARIABLES_SIMPLE_OVERRIDE)
    mark_as_advanced(FORCE  SIMPLE_OVERRIDE_BUILD_TEST
                            SIMPLE_OVERRIDE_BUILD_EXAMPLE)
else()
    mark_as_advanced(CLEAR  SIMPLE_OVERRIDE_BUILD_TEST
                            SIMPLE_OVERRIDE_BUILD_EXAMPLE)
endif()

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
    
    if(ssGUI_HIDE_VARIABLES_CLIP)
        mark_as_advanced(FORCE  CLIP_EXAMPLES
                                CLIP_TESTS
                                CLIP_X11_WITH_PNG
                                PNG_LIBRARY)
    else()
        mark_as_advanced(CLEAR  CLIP_EXAMPLES
                                CLIP_TESTS
                                CLIP_X11_WITH_PNG
                                PNG_LIBRARY)
    endif()

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
        set(FT_DISABLE_HARFBUZZ ON)  # Disabling FT_DISABLE_HARFBUZZ by default as it is causing errors
        set_target_properties(freetype PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
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
    #add_compile_options(/utf-8 /W2)
    target_compile_options(ssGUI PRIVATE /utf-8 /W1)
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
    if(ssGUI_HIDE_TARGET_CLIP)
        target_link_libraries(ssGUI PRIVATE clip)
    else()
        target_link_libraries(ssGUI PUBLIC clip)
    endif()
endif()

# Linking ssLogger to ssGUI
if(ssGUI_HIDE_TARGET_ssLOGGER)
    target_link_libraries(ssGUI PRIVATE ssLogger)
else()
    target_link_libraries(ssGUI PUBLIC ssLogger)
endif()


# ==========================================================
# Linking Backend to ssGUI and adding backend compiler definitions
# ==========================================================
if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OPTION_STRING}")
    if(ssGUI_MAIN_BACKEND STREQUAL "SFML")
        if(ssGUI_HIDE_TARGET_SFML)
            target_link_libraries(ssGUI PRIVATE SFML::Network SFML::Audio SFML::Graphics SFML::Window SFML::System)
        else()
            target_link_libraries(ssGUI PUBLIC SFML::Network SFML::Audio SFML::Graphics SFML::Window SFML::System)
        endif()
        
        target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_SFML)
    elseif(ssGUI_MAIN_BACKEND STREQUAL "Win32_OpenGL3_3")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/glad_v0.1.36")
        target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_WIN32_OPENGL)
        target_include_directories(ssGUI PRIVATE "${CMAKE_CURRENT_LIST_DIR}/External/wglExt")
        find_package(OpenGL REQUIRED)
        
        if(ssGUI_HIDE_TARGET_GLAD)
            target_link_libraries(ssGUI PRIVATE OpenGL::GL glad)
        else()
            target_link_libraries(ssGUI PUBLIC OpenGL::GL glad)
        endif()
        
        set_target_properties(glad PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
    elseif(ssGUI_MAIN_BACKEND STREQUAL "X11_OpenGL3_3")
        add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/External/glad_v0.1.36")
        target_compile_definitions(ssGUI PUBLIC SSGUI_MAIN_BACKEND_X11_OPENGL)
        find_package(OpenGL REQUIRED)
        find_package(X11 REQUIRED)
        
        if(ssGUI_HIDE_TARGET_GLAD)
            target_link_libraries(ssGUI PRIVATE OpenGL::GL X11::X11 X11::Xrandr X11::Xcursor glad)
        else()
            target_link_libraries(ssGUI PUBLIC OpenGL::GL X11::X11 X11::Xrandr X11::Xcursor glad)
        endif()
        
        set_target_properties(glad PROPERTIES FOLDER External)  # Set folder name for IDEs such as Visual Studio
    elseif(ssGUI_MAIN_BACKEND STREQUAL "Template")
        message(WARNING "Template for ssGUI_MAIN_BACKEND should only be used for compiling, running it will result a crash")
    else()
        message(FATAL_ERROR "Invalid Main Backend")
    endif()

    if(ssGUI_FONT_BACKEND STREQUAL "SFML")
        target_compile_definitions(ssGUI PUBLIC SSGUI_FONT_BACKEND_SFML)
    elseif(ssGUI_FONT_BACKEND STREQUAL "FreeType")
        
        if(ssGUI_HIDE_TARGET_FREETYPE)
            target_link_libraries(ssGUI PRIVATE freetype)
        else()
            target_link_libraries(ssGUI PUBLIC freetype)
        endif()
        
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
        
        if(ssGUI_HIDE_TARGET_STB_IMAGE)
            target_link_libraries(ssGUI PRIVATE ssGUI_stb_image)                        # INTERFACE doesn't work for whatever reason
        else()
            target_link_libraries(ssGUI PUBLIC ssGUI_stb_image)                        # INTERFACE doesn't work for whatever reason
        endif()
    
    elseif(ssGUI_FONT_BACKEND STREQUAL "Template")
        message(WARNING "Template for ssGUI_FONT_BACKEND should only be used for compiling, running it will result a crash")
    else()
        message(FATAL_ERROR "Invalid Image Backend")
    endif()
endif()

if(NOT ssGUI_MOCK_BACKEND_MODE STREQUAL "${ssGUI_MOCK_OFF_OPTION_STRING}")
    if(ssGUI_HIDE_TARGET_SIMPLE_OVERRIDE)
        target_link_libraries(ssGUI PRIVATE SimpleOverride)
    else()
        target_link_libraries(ssGUI PUBLIC SimpleOverride)
    endif()
    
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

#CopyResources(${CMAKE_CURRENT_BINARY_DIR})

if(ssGUI_BUILD_TESTS)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/Tests")
endif()

if(ssGUI_BUILD_EXAMPLES)
    add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/Src/Examples")
endif()