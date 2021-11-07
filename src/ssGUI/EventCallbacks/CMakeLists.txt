target_sources(ssGUI PRIVATE 
                    "${CMAKE_CURRENT_LIST_DIR}/BaseEventCallback.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/RecursiveChildrenAddedEventCallback.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/RecursiveChildrenRemovedEventCallback.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/MinMaxSizeChangedEventCallback.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/WindowCloseEventCallback.cpp"
                    "${CMAKE_CURRENT_LIST_DIR}/ButtonStateChangedEventCallback.cpp"
                    )