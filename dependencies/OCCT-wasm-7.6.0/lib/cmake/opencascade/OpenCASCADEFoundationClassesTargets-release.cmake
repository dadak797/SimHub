#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "TKernel" for configuration "Release"
set_property(TARGET TKernel APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKernel PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKernel.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKernel )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKernel "${_IMPORT_PREFIX}/lib/libTKernel.a" )

# Import target "TKMath" for configuration "Release"
set_property(TARGET TKMath APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(TKMath PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "C;CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/libTKMath.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS TKMath )
list(APPEND _IMPORT_CHECK_FILES_FOR_TKMath "${_IMPORT_PREFIX}/lib/libTKMath.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
