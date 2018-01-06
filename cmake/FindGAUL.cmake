# - Find GAUL
# Find the GAUL includes and client library
# This module defines
#  GAUL_LIBRARIES, the libraries needed to use GAUL.
#  GAUL_FOUND, If false, do not try to use GAUL.
#

if(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)
   set(GAUL_FOUND TRUE)

else(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)

  FIND_PATH(GAUL_INCLUDE_DIR gaul.h
      ${GAUL_ROOT}/include
      /usr/include
      /usr/local/include
      $ENV{ProgramFiles}/gaul/*/include
      $ENV{SystemDrive}/gaul/*/include
      )

message(STATUS "GAUL_INCLUDE_DIR=${GAUL_INCLUDE_DIR}")

  find_library(GAUL_LIBRARIES NAMES gaul libgaul
     PATHS
      ${GAUL_ROOT}/lib
     /usr/lib
     /usr/local/lib
     /usr/lib/gaul
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/gaul
     $ENV{ProgramFiles}/GAUL/*/lib/
     $ENV{SystemDrive}/GAUL/*/lib/
     )

message(STATUS "GAUL_LIBRARIES=${GAUL_LIBRARIES}")

  find_library(GAUL_UTIL_LIBRARIES NAMES gaul_util libgaul_util
     PATHS
      ${GAUL_ROOT}/lib
     /usr/lib
     /usr/local/lib
     /usr/lib/gaul
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/gaul
     $ENV{ProgramFiles}/GAUL/*/lib/
     $ENV{SystemDrive}/GAUL/*/lib/
     )

message(STATUS "GAUL_UTIL_LIBRARIES=${GAUL_UTIL_LIBRARIES}")

  if(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)
    set(GAUL_FOUND TRUE)
    message(STATUS "Found GAUL: ${GAUL_LIBRARIES}, ${GAUL_UTIL_LIBRARIES}")
  else(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)
    set(GAUL_FOUND FALSE)
    message(STATUS "GAUL not found.")
  endif(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)

  mark_as_advanced(GAUL_INCLUDE_DIR GAUL_LIBRARIES GAUL_UTIL_LIBRARIES)

endif(GAUL_INCLUDE_DIR AND GAUL_LIBRARIES AND GAUL_UTIL_LIBRARIES)
