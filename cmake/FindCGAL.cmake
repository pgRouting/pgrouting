# - Find CGAL
# Find the CGAL includes and client library
# This module defines
#  CGAL_INCLUDE_DIR, where to find CGAL.h
#  CGAL_LIBRARIES, the libraries needed to use CGAL.
#  CGAL_FOUND, If false, do not try to use CGAL.

if(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)
   set(CGAL_FOUND TRUE)

else(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)

 FIND_PATH(CGAL_INCLUDE_DIR CGAL/basic.h
      ${CGAL_ROOT}/include
      /usr/include
      /usr/local/include
      $ENV{ProgramFiles}/CGAL/*/include
      $ENV{SystemDrive}/CGAL/*/include
      )

  find_library(CGAL_LIBRARIES NAMES CGAL libCGAL
     PATHS
      ${CGAL_ROOT}/lib
     /usr/lib
     /usr/local/lib
     /usr/lib/CGAL
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/CGAL
     $ENV{ProgramFiles}/CGAL/*/lib
     $ENV{SystemDrive}/CGAL/*/lib
     )

#  set(Boost_DEBUG ON)
  find_package(Boost COMPONENTS thread REQUIRED)
  if(Boost_FOUND)
    set(BOOST_THREAD_LIBRARIES ${Boost_LIBRARIES})
  endif(Boost_FOUND)

  # check boost version we may need other components
  if("${Boost_VERSION}" VERSION_GREATER "104900")
      find_package(Boost COMPONENTS thread system REQUIRED)
      if(Boost_FOUND)
        set(BOOST_THREAD_LIBRARIES ${Boost_LIBRARIES})
      endif(Boost_FOUND)
  endif("${Boost_VERSION}" VERSION_GREATER "104900")

  find_library(GMP_LIBRARIES NAMES gmp libgmp
     PATHS
      ${GMP_ROOT}/lib
     /usr/lib
     /usr/local/lib
     /usr/lib/gmp
     /usr/lib64
     /usr/local/lib64
     /usr/lib64/gmp
     $ENV{ProgramFiles}/gmp/*/lib
     $ENV{SystemDrive}/gmp/*/lib
     )

message(STATUS "CGAL_INCLUDE_DIR=${CGAL_INCLUDE_DIR}")
message(STATUS "CGAL_LIBRARIES=${CGAL_LIBRARIES}")
message(STATUS "BOOST_THREAD_LIBRARIES=${BOOST_THREAD_LIBRARIES}")
message(STATUS "GMP_LIBRARIES=${GMP_LIBRARIES}")

  if(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)
    set(CGAL_FOUND TRUE)
    message(STATUS "Found CGAL: ${CGAL_INCLUDE_DIR}, ${CGAL_LIBRARIES}, ${BOOST_THREAD_LIBRARIES}, ${GMP_LIBRARIES}")
    INCLUDE_DIRECTORIES(${CGAL_INCLUDE_DIR} $ENV{CGAL_CFG})
  else(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)
    set(CGAL_FOUND FALSE)
    message(STATUS "CGAL not found.")
  endif(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)

  mark_as_advanced(CGAL_INCLUDE_DIR CGAL_LIBRARIES BOOST_THREAD_LIBRARIES GMP_LIBRARIES)

endif(CGAL_INCLUDE_DIR AND CGAL_LIBRARIES AND BOOST_THREAD_LIBRARIES AND GMP_LIBRARIES)
