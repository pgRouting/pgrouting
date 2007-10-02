# - Find CGAL
# Find the CGAL includes and client library
# This module defines
#  CGAL_INCLUDE_DIR, where to find CGAL.h
#  CGAL_LIBRARIES, the libraries needed to use CGAL.
#  CGAL_FOUND, If false, do not try to use CGAL.

if(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
   set(CGAL_FOUND TRUE)

else(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)

 FIND_PATH(CGAL_INCLUDE_DIR basic.h
      /usr/include/CGAL
      /usr/local/include/CGAL
      $ENV{ProgramFiles}/CGAL/*/include/CGAL
      $ENV{SystemDrive}/CGAL/*/include/CGAL
      )

  find_library(CGAL_LIBRARIES NAMES CGAL libCGAL
     PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/CGAL
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/CGAL
     $ENV{ProgramFiles}/CGAL/*/lib/ms
     $ENV{SystemDrive}/CGAL/*/lib/ms
     )
      
  if(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
    set(CGAL_FOUND TRUE)
    message(STATUS "Found CGAL: ${CGAL_INCLUDE_DIR}, ${CGAL_LIBRARIES}")
    INCLUDE_DIRECTORIES(${CGAL_INCLUDE_DIR} $ENV{CGAL_CFG})
  else(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
    set(CGAL_FOUND FALSE)
    message(STATUS "CGAL not found.")
  endif(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)

  mark_as_advanced(CGAL_INCLUDE_DIR CGAL_LIBRARIES)

endif(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES)
