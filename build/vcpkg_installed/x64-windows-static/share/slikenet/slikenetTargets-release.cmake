#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "SLikeNetLibStatic" for configuration "Release"
set_property(TARGET SLikeNetLibStatic APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(SLikeNetLibStatic PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/SLikeNetLibStatic.lib"
  )

list(APPEND _cmake_import_check_targets SLikeNetLibStatic )
list(APPEND _cmake_import_check_files_for_SLikeNetLibStatic "${_IMPORT_PREFIX}/lib/SLikeNetLibStatic.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
