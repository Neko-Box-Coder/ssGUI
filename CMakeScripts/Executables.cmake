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

        if(CUR_NAME STREQUAL "CustomGraphicsExampleV2")
            target_link_libraries("${CUR_NAME}" PRIVATE OpenGL::GL glad)
        endif()

        # Link targets
        foreach(linkTarget ${targetsToLink})
            target_link_libraries("${CUR_NAME}" PRIVATE ${linkTarget})
        endforeach()

        # Sub-directory for build
        set_target_properties("${CUR_NAME}" PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${binSubFolder})

        # Set folder name for IDEs such as Visual Studio
        set_target_properties("${CUR_NAME}" PROPERTIES FOLDER ${ideFolderName})

    endforeach(CUR_SOURCE_PATH "${ssGUI_EXE_CPP_FILES}")

    #CopyResources(${binSubFolder})

endfunction()