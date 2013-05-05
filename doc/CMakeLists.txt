# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

set(SPHINX_THEME "cloud")
#set(SPHINX_THEME_DIR "_themes")

if(WITH_DOC)

    find_package(Sphinx REQUIRED)

    if(NOT DEFINED SPHINX_THEME)
        set(SPHINX_THEME default)
    endif()

    if(NOT DEFINED SPHINX_THEME_DIR)
        set(SPHINX_THEME_DIR)
    endif()

    # configured documentation tools and intermediate build results
    set(BINARY_BUILD_DIR "${PGROUTING_BINARY_DIR}/doc/_build")

    # Sphinx cache with pickled ReST documents
    set(SPHINX_CACHE_DIR "${PGROUTING_BINARY_DIR}/doc/_doctrees")

    # HTML output directory
    set(SPHINX_HTML_DIR "${PGROUTING_BINARY_DIR}/html")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
        @ONLY)

    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/index.html" DESTINATION "${SPHINX_HTML_DIR}")

    add_custom_target(pgRoutingDocumentationdoc ALL
        ${SPHINX_EXECUTABLE}
        -q -b html
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_HTML_DIR}"
        COMMENT "Building HTML documentation with Sphinx")

endif(WITH_DOC)
