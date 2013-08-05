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
 
    find_package(Sphinx)

    if (NOT SPHINX_FOUND)
        message(WARNING "Sphinx not found. Cannot generate documentation!")
    else()
        if (SPHINX_VERSION VERSION_LESS 1.0)
            message(WARNING "Your Sphinx version is too old!
                This project requires Sphinx v1.0 or above to produce
                proper documentation (you have v${SPHINX_VERSION}).
                You will get output but it will have errors.")
        endif()
 
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
 
        # Sphinx cache with pickled ReST documents
        set(SPHINX_BUILD_DIR "${PGROUTING_BINARY_DIR}/doc")
 
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${BINARY_BUILD_DIR}")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${BINARY_BUILD_DIR}")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CNAME" DESTINATION "${SPHINX_BUILD_DIR}")
 
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in"
            "${SPHINX_BUILD_DIR}/html/index.html")
 
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
            "${SPHINX_BUILD_DIR}/html/en/index.html")
 
        # Add documentation to targets
        set(DOC_TARGETS html)
 
        option(BUILD_MAN "Create a target for building man pages." ON)
 
        if (BUILD_MAN)
            if (SPHINX_VERSION VERSION_LESS 1.0)
                message(WARNING "Sphinx version 1.0 > is required to build man pages. You have v${SPHINX_VERSION}.")
            else()
                list(APPEND DOC_TARGETS man)
            endif()
        endif()
 
        option(BUILD_LATEX "Create a target for building latex docs (to create PDF)." ON)
 
        if (BUILD_LATEX)
            find_package(LATEX)
 
            if (NOT LATEX_COMPILER)
                message("Couldn't find Latex, can't build latex docs using Sphinx")
            else()
                list(APPEND DOC_TARGETS latex)
            endif()
        endif()
 
        # The doc target will build all documentation targets.
        add_custom_target(doc)
 
        # Localization output directory
        # set(SPHINX_I18N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/i18n/pot")
 
        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
            "${BINARY_BUILD_DIR}/conf.py"
            @ONLY)
 
        foreach (DOC_TARGET ${DOC_TARGETS})
            add_custom_target(${DOC_TARGET}
                ${SPHINX_EXECUTABLE}
                # -q   # Enable for quiet mode
                # -Q   # Enable for more quiet mode
                -b ${DOC_TARGET}
                -d "${SPHINX_CACHE_DIR}"
                -c "${BINARY_BUILD_DIR}" # enable if using cmake-generated conf.py
                "${CMAKE_CURRENT_SOURCE_DIR}/doc"
                "${SPHINX_BUILD_DIR}/${DOC_TARGET}"
                COMMENT "Building ${DOC_TARGET} documentation with Sphinx")
 
            add_dependencies(doc ${DOC_TARGET})
        endforeach()
 
        if(LATEX_COMPILER)
            add_custom_target(pdf
                pdflatex
                -interaction=nonstopmode
                "${PGROUTING_BINARY_DIR}/doc/latex/en/pgRoutingDocumentation.tex"
                COMMENT "Converting to PDF format with Sphinx")
 
            add_dependencies(doc pdf)
            add_dependencies(pdf latex)
            list(APPEND DOC_TARGETS pdf)
        endif()
 
        message("Building documentation enabled for: ${DOC_TARGETS}")
    endif()
 
endif(WITH_DOC)

