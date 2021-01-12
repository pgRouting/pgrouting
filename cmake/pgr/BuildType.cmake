
# Set a default build type if none was specified
set(PROJECT_DEFAULT_BUILD_TYPE "Release")

if (CMAKE_BUILD_TYPE AND
        NOT CMAKE_BUILD_TYPE MATCHES "^(Debug|Release|RelWithDebInfo|MinSizeRel)$")
    message(FATAL_ERROR "Invalid value for CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}
    valid values are Debug|Release|RelWithDebInfo|MinSizeRel")
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to '${PROJECT_DEFAULT_BUILD_TYPE}' as none was specified.")
    set(CMAKE_BUILD_TYPE "${PROJECT_DEFAULT_BUILD_TYPE}" CACHE
        STRING "Choose the type of build." FORCE)
endif()
message(STATUS "CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE}")
