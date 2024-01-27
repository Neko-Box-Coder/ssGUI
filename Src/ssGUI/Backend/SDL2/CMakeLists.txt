target_sources(ssGUI PRIVATE 
                    "${CMAKE_CURRENT_LIST_DIR}/BackendDrawingSDL2.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendSystemInputSDL2.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendMainWindowSDL2.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/SDL2InputConverter.cpp"
                    )