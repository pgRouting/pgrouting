#


option(BUILD_DOXY
    "Set ON|OFF (default=OFF) to build Developers Documentation" OFF)

if (BUILD_DOXY)
    find_package(Doxygen ${DOXYGEN_MINIMUM_VERSION})

    if(Doxygen_FOUND)
        message(STATUS "DOXYGEN_VERSION=${DOXYGEN_VERSION}")
        configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
        doxygen_add_docs(doxy ALL USE_STAMP_FILE
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
            COMMENT "Generating API documentation with Doxygen"
            CONFIG_FILE ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        )
    endif()

endif()
