# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

OPTION(WITH_DOC "Build Documentation" OFF)

IF(WITH_DOC)

    FIND_PACKAGE(Sphinx REQUIRED)

    if(NOT DEFINED SPHINX_THEME)
        SET(SPHINX_THEME default)
    endif()

    if(NOT DEFINED SPHINX_THEME_DIR)
        SET(SPHINX_THEME_DIR)
    endif()

    # configured documentation tools and intermediate build results
    SET(BINARY_BUILD_DIR "${PGROUTING_BINARY_DIR}/doc/_build")

    # Sphinx cache with pickled ReST documents
    SET(SPHINX_CACHE_DIR "${PGROUTING_BINARY_DIR}/doc/_doctrees")

    # HTML output directory
    SET(SPHINX_HTML_DIR "${PGROUTING_BINARY_DIR}/html")

    CONFIGURE_FILE(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
        @ONLY)

    ADD_CUSTOM_TARGET(pgRoutingDocumentationdoc ALL
        ${SPHINX_EXECUTABLE}
        -q -b html
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${CMAKE_CURRENT_SOURCE_DIR}"
        "${SPHINX_HTML_DIR}"
        COMMENT "Building HTML documentation with Sphinx")

ENDIF(WITH_DOC)