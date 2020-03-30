#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "celero" for configuration "Release"
set_property(TARGET celero APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(celero PROPERTIES
  IMPORTED_LOCATION_RELEASE "/usr/local/lib/libcelero.so"
  IMPORTED_SONAME_RELEASE "libcelero.so"
  )

list(APPEND _IMPORT_CHECK_TARGETS celero )
list(APPEND _IMPORT_CHECK_FILES_FOR_celero "/usr/local/lib/libcelero.so" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
