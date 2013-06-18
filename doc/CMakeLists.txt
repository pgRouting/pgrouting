# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

set(SPHINX_THEME "haiku")
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

    # PDF output directory
    set(SPHINX_LATEX_DIR "${PGROUTING_BINARY_DIR}/latex")

    # MAN output directory
    set(SPHINX_MAN_DIR "${PGROUTING_BINARY_DIR}/man")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
        @ONLY)

    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CNAME" DESTINATION "${PGROUTING_BINARY_DIR}")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in"
        "${PGROUTING_BINARY_DIR}/index.html")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
        "${SPHINX_HTML_DIR}/index.html")

    add_custom_target(pgRoutingDocumentationdoc ALL
        ${SPHINX_EXECUTABLE}
        -q -b html
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_HTML_DIR}"
        COMMENT "Building HTML documentation with Sphinx")

    add_custom_target(pgRoutingDocumentationman ALL
        ${SPHINX_EXECUTABLE}
        -q -b man
        -c "${BINARY_BUILD_DIR}"
        -E
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_MAN_DIR}"
        COMMENT "Building MAN documentation with Sphinx")

    add_custom_target(pgRoutingDocumentationtex ALL
        ${SPHINX_EXECUTABLE}
        -q -b latex
        -c "${BINARY_BUILD_DIR}"
        -E
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_LATEX_DIR}"
        COMMENT "Building LATEX documentation with Sphinx")

    add_custom_command(
        TARGET pgRoutingDocumentationtex POST_BUILD
        COMMAND cd ${SPHINX_LATEX_DIR} && pdflatex -interaction=nonstopmode pgRoutingDocumentation.tex > /dev/null 2>&1
        COMMENT "Converting to PDF format with Sphinx")

endif(WITH_DOC)
