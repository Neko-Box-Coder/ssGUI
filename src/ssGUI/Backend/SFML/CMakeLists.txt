target_sources(ssGUI PRIVATE 
                    "${CMAKE_CURRENT_LIST_DIR}/BackendDrawingSFML.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendFontSFML.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendImageSFML.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendSystemInputSFML.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendMainWindowSFML.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/SFMLInputConverter.cpp"
                    )