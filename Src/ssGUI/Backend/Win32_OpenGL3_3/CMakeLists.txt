target_sources(ssGUI PRIVATE 
                    "${CMAKE_CURRENT_LIST_DIR}/BackendDrawingWin32_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendSystemInputWin32_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendMainWindowWin32_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/Win32InputConverter.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/../OpenGL3_3_Common/OpenGL3_3_Common.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/../DynamicImageAtlas.cpp"
                    )