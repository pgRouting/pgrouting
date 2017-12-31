#
# Based on:
# https://majewsky.wordpress.com/2010/08/14/tip-of-the-day-cmake-and-doxygen/
#

option(BUILD_DOXY
    "Set ON|OFF (default=OFF) to build Developers Documentation" ON)

if (BUILD_DOXY AND WITH_DOC)
    message(STATUS "Developers documentation.")
    find_package(Doxygen ${DOXYGEN_MINIMUM_VERSION})

    if(DOXYGEN_FOUND)
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        add_custom_target(doxy
            ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen" VERBATIM
            )
        message(".....OK")
    endif()

endif()
