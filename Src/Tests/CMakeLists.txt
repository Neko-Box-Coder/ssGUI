include("${CMAKE_CURRENT_LIST_DIR}/../../External/Embed2C/embedFile.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/../../CMakeScripts/Executables.cmake")


# Check if test resources is populated
file(SIZE "${CMAKE_CURRENT_LIST_DIR}/TestsResources.c" EMBEDDED_FILE_SIZE)
if(EMBEDDED_FILE_SIZE LESS 1024)
    set(TESTS_RESOURCES_TO_EMBED    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/arial.ttf"
                                    "ssGUI_Test_arial"
                                    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/NotoColorEmoji.ttf"
                                    "ssGUI_Test_NotoColorEmoji"
                                    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/sd.png"
                                    "ssGUI_Test_sd"
                                    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/sdTestEdge.png"
                                    "ssGUI_Test_sd_edge"
                                    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/NotoSans-Bold.ttf"
                                    "ssGUI_Test_NotoSans_Bold")
    
    EMBED_FILES("${EMBED_EXEC_PATH}" 
                "${CMAKE_CURRENT_LIST_DIR}/TestsResources.c"
                "${TESTS_RESOURCES_TO_EMBED}")

    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/TestsResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        message(WARNING "Failed to embed test resources")
    endif()
endif()

add_library(ssGUI_Test_Resources "${CMAKE_CURRENT_LIST_DIR}/TestsResources.c")
target_include_directories(ssGUI_Test_Resources PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
set_target_properties(ssGUI_Test_Resources PROPERTIES FOLDER "Tests")

# ----------------------------------------------------------
# Auto Tests
# ----------------------------------------------------------
#if(NOT ssGUI_CAN_OVER_BACKEND)

    #AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/BackendTests/" 
    #                        "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/BackendTests/" 
    #                        "Tests/AutoTests/BackendTests"
    #                        "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")
#else()
#    message(WARNING "ssGUI_CAN_OVER_BACKEND is set to True, skipping backend tests")
#    message(WARNING "Set ssGUI_CAN_OVER_BACKEND to not be True in order to produce backend tests")
#endif()

if(ssGUI_CAN_OVER_BACKEND)
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/BackendTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/BackendTests/" 
                            "Tests/AutoTests/BackendTests"
                            "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger;CppOverride")
else()
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/BackendTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/BackendTests/" 
                            "Tests/AutoTests/BackendTests"
                            "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")
endif()


# TODO(NOW): Uncomment this when finished testing backend
#if(ssGUI_CAN_OVER_BACKEND)
#    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/EventCallbackTests/" 
#                            "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/EventCallbackTests/" 
#                            "Tests/AutoTests/EventCallbackTests"
#                            "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger;CppOverride")
    
#    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/GUIObjectTests/" 
#                            "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/GUIObjectTests/" 
#                            "Tests/AutoTests/GUIObjectTests"
#                            "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger;CppOverride")    
#else()
#    message(WARNING "ssGUI_CAN_OVER_BACKEND is set to False, skipping tests")
#endif()

if(WIN32)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/")
    file(   COPY "${CMAKE_CURRENT_LIST_DIR}/AutoTests/RunAllTests.bat"
            DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/AutoTests")
    #execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_LIST_DIR}/AutoTests/RunAllTests.bat" "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/RunAllTests.bat")
else()
    file(   CHMOD "${CMAKE_CURRENT_LIST_DIR}/AutoTests/RunAllTests.sh"
            PERMISSIONS  OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_WRITE WORLD_EXECUTE)
    file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/")
    execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CMAKE_CURRENT_LIST_DIR}/AutoTests/RunAllTests.sh" "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/RunAllTests.sh")
endif()

# ----------------------------------------------------------
# Manual Tests
# ----------------------------------------------------------
if(NOT ssGUI_CAN_OVER_BACKEND)
    AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/ManualTests/BackendTests/" 
                            "${CMAKE_CURRENT_BINARY_DIR}/ManualTests/BackendTests/" 
                            "Tests/ManualTests/BackendTests"
                            "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")
else()
    message(WARNING "ssGUI_CAN_OVER_BACKEND is set to True, skipping manual backend tests")
    message(WARNING "Set ssGUI_CAN_OVER_BACKEND to not be True in order to produce manual backend tests")
endif()

# TODO: The aim is to replace these manual tests with auto tests, unless it is backend tests I guess
# TODO(NOW): Uncomment this when finished testing backend
#AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/ManualTests/EventCallbackTests/" 
#                        "${CMAKE_CURRENT_BINARY_DIR}/ManualTests/EventCallbackTests/" 
#                        "Tests/ManualTests/EventCallbackTests"
#                        "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")

#AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/ManualTests/GUIObjectTests/" 
#                        "${CMAKE_CURRENT_BINARY_DIR}/ManualTests/GUIObjectTests/" 
#                        "Tests/ManualTests/GUIObjectTests"
#                        "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")

# ----------------------------------------------------------
# Tests Templates
# ----------------------------------------------------------

# TODO(NOW): Uncomment this when finished testing backend
#AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/AutoTests/" 
#                        "${CMAKE_CURRENT_BINARY_DIR}/AutoTests/" 
#                        "Tests/AutoTests"
#                        "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")

#AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/ManualTests/" 
#                        "${CMAKE_CURRENT_BINARY_DIR}/ManualTests/" 
#                        "Tests/ManualTests"
#                        "ssGUI;ssGUI_Test_Resources;ssTest;ssLogger")
