target_sources(ssGUI PRIVATE 
                    "${CMAKE_CURRENT_LIST_DIR}/BackendDrawingX11_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendSystemInputX11_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/BackendMainWindowX11_OpenGL3_3.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/X11InputConverter.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/../OpenGL3_3_Common/OpenGL3_3_Common.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/../DynamicImageAtlas.cpp"
                    )