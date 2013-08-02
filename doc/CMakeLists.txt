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
    set(SPHINX_HTML_DIR "${PGROUTING_BINARY_DIR}/doc/html")

    # PDF output directory
    set(SPHINX_LATEX_DIR "${PGROUTING_BINARY_DIR}/doc/latex")

    # MAN output directory
    set(SPHINX_MAN_DIR "${PGROUTING_BINARY_DIR}/doc/man")

    # Localization output directory
    set(SPHINX_I18N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/i18n/pot")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${BINARY_BUILD_DIR}/conf.py"
        @ONLY)

    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CNAME" DESTINATION "${PGROUTING_BINARY_DIR}")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in"
        "${SPHINX_HTML_DIR}/index.html")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
        "${SPHINX_HTML_DIR}/en/index.html")

    add_custom_target(doc_gettext ALL
        ${SPHINX_EXECUTABLE}
        -Q -a -E -b gettext
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_I18N_DIR}"
        COMMENT "SPHINX build finished. The message catalogs are in ${SPHINX_I18N_DIR}.")

    add_custom_target(doc_html ALL
        ${SPHINX_EXECUTABLE}
        -Q -a -E -b html
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_HTML_DIR}/en"
        COMMENT "SPHINX build finished. The HTML pages are in ${SPHINX_HTML_DIR}/en.")

    add_custom_target(doc_man ALL
        ${SPHINX_EXECUTABLE}
        -Q -a -E -b man
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_MAN_DIR}/en"
        COMMENT "SPHINX build finished. The manual pages are in ${SPHINX_MAN_DIR}/en.")

    add_custom_target(doc_latex ALL
        ${SPHINX_EXECUTABLE}
        -Q -a -E -b latex
        -c "${BINARY_BUILD_DIR}"
        -d "${SPHINX_CACHE_DIR}"
        "${PGROUTING_SOURCE_DIR}"
        "${SPHINX_LATEX_DIR}/en"
        COMMENT "SPHINX build finished; the LaTeX files are in ${SPHINX_LATEX_DIR}/en.")

    add_custom_command(
        TARGET doc_latex POST_BUILD
        COMMAND cd "${SPHINX_LATEX_DIR}/en" && pdflatex -interaction=nonstopmode pgRoutingDocumentation.tex > /dev/null 2>&1
        COMMENT "Converting to PDF format with Sphinx")

    add_dependencies(doc_html  doc_gettext)
    add_dependencies(doc_man   doc_gettext)
    add_dependencies(doc_latex doc_gettext)

endif(WITH_DOC)
