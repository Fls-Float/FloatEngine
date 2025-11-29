#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "MINIZIP::minizip-ng" for configuration "Release"
set_property(TARGET MINIZIP::minizip-ng APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(MINIZIP::minizip-ng PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/minizip-ng.lib"
  )

list(APPEND _cmake_import_check_targets MINIZIP::minizip-ng )
list(APPEND _cmake_import_check_files_for_MINIZIP::minizip-ng "${_IMPORT_PREFIX}/lib/minizip-ng.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
