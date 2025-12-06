#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "unofficial::enet::enet" for configuration "Release"
set_property(TARGET unofficial::enet::enet APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(unofficial::enet::enet PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/enet.lib"
  )

list(APPEND _cmake_import_check_targets unofficial::enet::enet )
list(APPEND _cmake_import_check_files_for_unofficial::enet::enet "${_IMPORT_PREFIX}/lib/enet.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
