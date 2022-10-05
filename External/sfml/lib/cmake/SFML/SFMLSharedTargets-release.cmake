#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SFML::System" for configuration "Release"
set_property(TARGET SFML::System APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::System PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsfml-system.so.3.0.0"
  IMPORTED_SONAME_RELEASE "libsfml-system.so.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::System )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::System "${_IMPORT_PREFIX}/lib/libsfml-system.so.3.0.0" )

# Import target "SFML::Window" for configuration "Release"
set_property(TARGET SFML::Window APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::Window PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsfml-window.so.3.0.0"
  IMPORTED_SONAME_RELEASE "libsfml-window.so.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Window )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Window "${_IMPORT_PREFIX}/lib/libsfml-window.so.3.0.0" )

# Import target "SFML::Network" for configuration "Release"
set_property(TARGET SFML::Network APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::Network PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsfml-network.so.3.0.0"
  IMPORTED_SONAME_RELEASE "libsfml-network.so.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Network )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Network "${_IMPORT_PREFIX}/lib/libsfml-network.so.3.0.0" )

# Import target "SFML::Graphics" for configuration "Release"
set_property(TARGET SFML::Graphics APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::Graphics PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsfml-graphics.so.3.0.0"
  IMPORTED_SONAME_RELEASE "libsfml-graphics.so.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Graphics )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Graphics "${_IMPORT_PREFIX}/lib/libsfml-graphics.so.3.0.0" )

# Import target "SFML::Audio" for configuration "Release"
set_property(TARGET SFML::Audio APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SFML::Audio PROPERTIES
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libsfml-audio.so.3.0.0"
  IMPORTED_SONAME_RELEASE "libsfml-audio.so.3.0"
  )

list(APPEND _IMPORT_CHECK_TARGETS SFML::Audio )
list(APPEND _IMPORT_CHECK_FILES_FOR_SFML::Audio "${_IMPORT_PREFIX}/lib/libsfml-audio.so.3.0.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
