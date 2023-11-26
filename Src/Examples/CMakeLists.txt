include("${CMAKE_CURRENT_LIST_DIR}/../../CMakeScripts/Executables.cmake")
    
# Check if example resources is populated
file(SIZE "${CMAKE_CURRENT_LIST_DIR}/ExamplesResources.c" EMBEDDED_FILE_SIZE)
if(EMBEDDED_FILE_SIZE LESS 1024)
    set(EXAMPLE_RESOURCES_TO_EMBED  "${CMAKE_CURRENT_LIST_DIR}/../../Resources/sd.png"
                                    "ssGUI_Example_sd"
                                    "${CMAKE_CURRENT_LIST_DIR}/../../Resources/NotoSans-Bold.ttf"
                                    "ssGUI_Example_NotoSans_Bold")
    
    EMBED_FILES("${EMBED_EXEC_PATH}" 
                "${CMAKE_CURRENT_LIST_DIR}/ExamplesResources.c"
                "${EXAMPLE_RESOURCES_TO_EMBED}")

    file(SIZE "${CMAKE_CURRENT_LIST_DIR}/ExamplesResources.c" EMBEDDED_FILE_SIZE)
    if(EMBEDDED_FILE_SIZE LESS 1024)
        message(WARNING "Failed to embed example resources")
    endif()
endif()

add_library(ssGUI_Example_Resources "${CMAKE_CURRENT_LIST_DIR}/ExamplesResources.c")
target_include_directories(ssGUI_Example_Resources PUBLIC "${CMAKE_CURRENT_LIST_DIR}")
set_target_properties(ssGUI_Example_Resources PROPERTIES FOLDER "Examples")

AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/" 
                        "${CMAKE_CURRENT_BINARY_DIR}/" 
                        "Examples"
                        "ssGUI;ssGUI_Example_Resources;ssLogger")

AddFilesToExecutables(  "${CMAKE_CURRENT_LIST_DIR}/Playground/" 
                        "${CMAKE_CURRENT_BINARY_DIR}/Playground/" 
                        "Examples/Playground"
                        "ssGUI;ssGUI_Example_Resources;ssLogger")