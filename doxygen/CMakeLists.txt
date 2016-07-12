#
# Based on:
# https://majewsky.wordpress.com/2010/08/14/tip-of-the-day-cmake-and-doxygen/
#

option(WITH_DOXY
    "Set ON|OFF (default=OFF) to build Developers Documentation" OFF)

if (WITH_DOC AND WITH_DOXY)
    find_package(Doxygen REQUIRED)

    if(NOT DOXYGEN_FOUND)
        message(FATAL_ERROR "Doxygen is needed to build the documentation.")
    elseif()

        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doxy
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
    endif()

endif()
