# - Find GAUL
# Find the GAUL includes and client library
# This module defines
#  GAUL_LIBRARIES, the libraries needed to use GAUL.
#  GAUL_FOUND, If false, do not try to use GAUL.
#

if(GAUL_LIBRARIES)
   set(GAUL_FOUND TRUE)

else(GAUL_LIBRARIES)

  find_library(GAUL_LIBRARIES NAMES gaul gaul_util libgaul libgaul_util
     PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/gaul
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/gaul
     $ENV{ProgramFiles}/GAUL/*/lib/ms
     $ENV{SystemDrive}/GAUL/*/lib/ms
     )
      
  if(GAUL_LIBRARIES)
    set(GAUL_FOUND TRUE)
    message(STATUS "Found GAUL: ${GAUL_LIBRARIES}")
  else(GAUL_LIBRARIES)
    set(GAUL_FOUND FALSE)
    message(STATUS "GAUL not found.")
  endif(GAUL_LIBRARIES)

  mark_as_advanced(GAUL_LIBRARIES)

endif(GAUL_LIBRARIES)
