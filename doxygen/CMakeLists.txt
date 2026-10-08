# This file is part of the pgRouting project.
# Copyright (c) 2016-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE
# Main changes
# developers is build by default unless other wise stated

option(BUILD_DOXY "Set ON|OFF (default=ON) to build Developers Documentation" ON)

if (BUILD_DOXY)
    find_package(Doxygen ${DOXYGEN_MINIMUM_VERSION})

    if(Doxygen_FOUND)
        message(STATUS "DOXYGEN_VERSION=${DOXYGEN_VERSION}")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        file(GLOB_RECURSE DOXY_SOURCES CONFIGURE_DEPENDS
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/src/*.cc
            ${CMAKE_SOURCE_DIR}/src/*.cpp
            ${CMAKE_SOURCE_DIR}/src/*.h
            ${CMAKE_SOURCE_DIR}/src/*.hpp
            ${CMAKE_SOURCE_DIR}/include/*.h
            ${CMAKE_SOURCE_DIR}/include/*.hpp
        )
        list(APPEND DOXY_SOURCES ${CMAKE_SOURCE_DIR}/README.md)
        doxygen_add_docs(doxy ${DOXY_SOURCES}
            ALL USE_STAMP_FILE
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        )
    endif()

endif()
