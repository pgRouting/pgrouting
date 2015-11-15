# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
# Vicky Vergara 11/2015
#


#-----------------------------------------------------------------------------
# Checking DOCUMENTATION requierments 
#-----------------------------------------------------------------------------

if (NOT (BUILD_HTML OR BUILD_LATE OR BUILD_MAN))
    set (WITH_DOC OFF)
endif()

if(WITH_DOC)

    message("Checking Documentation Requierments")


    #----------------------
    # Sphinx
    #----------------------
    find_package(Sphinx)

    if (NOT SPHINX_FOUND)
        set(WITH_DOC OFF)
        message(WARNING "Sphinx not found. Can not generate documentation!
        ..... Skipping")
    else()

        if (SPHINX_VERSION VERSION_LESS ${SPHINX_MINIMUM_VERSION})
            message(WARNING "Your Sphinx version is too old!
            This project requires Sphinx v1.0 or above to produce
            poper documentation (you have v${SPHINX_VERSION}).
            You will get output but it will have errors.")
        endif()

        if (BUILD_LATEX)
            find_package(LATEX)

            if (NOT PDFLATEX_COMPILER OR NOT LATEX_COMPILER)
                message(WARNING "Couldn't find Latex, can't build latex docs using Sphinx")
                set(BUILD_LATEX OFF)
            endif()
        endif()

        set(SPHINX_THEME "haiku")
        set(SPHINX_BINARY_BUILD_DIR "${PGROUTING_BINARY_DIR}/doc/_build")
        set(SPHINX_CACHE_DIR "${PGROUTING_BINARY_DIR}/doc/_doctrees")


        if (CDEBUG)
            message(STATUS "  SPHINX_VERSION=${SPHINX_VERSION}")
            message(STATUS "  SPHINX_THEME=${SPHINX_THEME}")
            message(STATUS "  SPHINX_BINARY_BUILD_DIR = ${SPHINX_BINARY_BUILD_DIR}")
            message(STATUS "  SPHINX_CACHE_DIR = ${SPHINX_CACHE_DIR}")
            if (BUILD_LATEX)
                message(STATUS "  PDFLATEX_COMPILER = ${PDFLATEX_COMPILER}")
                message(STATUS "  LATEX_COMPILER = ${LATEX_COMPILER}")
            endif()
        endif()

    message(".....OK")
    endif()


endif()

if (WITH_DOC)

    message("Setting up Documentation")

    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${SPHINX_BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${SPHINX_BINARY_BUILD_DIR}")
    file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CNAME" DESTINATION "${PGROUTING_BINARY_DIR}")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in"
        "${PGROUTING_BINARY_DIR}/doc/html/index.html")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
        "${PGROUTING_BINARY_DIR}/doc/html/en/index.html")

    # Add documentation to targets
    set(DOC_TARGETS html)

    #        option(BUILD_MAN "Create a target for building man pages." ON)

    if (BUILD_MAN)
        if (SPHINX_VERSION VERSION_LESS 1.0)
            message(WARNING "Sphinx version 1.0 > is required to build man pages. You have v${SPHINX_VERSION}.")
        else()
            list(APPEND DOC_TARGETS man)
        endif()
    endif()


    if (BUILD_LATEX)
        list(APPEND DOC_TARGETS latex)
    endif()

    # The doc target will build all documentation targets.
    add_custom_target(doc #ALL
        COMMENT "Building documentation with Sphinx")

    # Localization output directory
    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
        "${SPHINX_BINARY_BUILD_DIR}/conf.py"
        @ONLY)

    foreach (DOC_TARGET ${DOC_TARGETS})
        add_custom_command(
            TARGET doc POST_BUILD
            COMMAND ${SPHINX_EXECUTABLE} 
            -q   # Enable for quiet mode 
            -Q  # Enable for even more quiet mode
            -b ${DOC_TARGET}
            -E  # don't use a saved environment, always read all file
            -a  # write all files; default is to only write new and changed files
            -d "${SPHINX_CACHE_DIR}" 
            -c "${SPHINX_BINARY_BUILD_DIR}" 
            "${PGROUTING_SOURCE_DIR}" 
            "${PGROUTING_BINARY_DIR}/doc/${DOC_TARGET}/en"
            COMMENT "Generating ${DOC_TARGET} documentation ...")
    endforeach()

    if(BUILD_LATEX)
        add_custom_command(
            TARGET doc POST_BUILD
            COMMAND ${LATEX_COMPILER} 
            -interaction=nonstopmode 
            "pgRoutingDocumentation.tex" > /dev/null 2>&1
            WORKING_DIRECTORY "${PGROUTING_BINARY_DIR}/doc/latex/en"
            COMMENT "Converting Latex to PDF format")

        list(APPEND DOC_TARGETS pdf)
    endif()

    message("-- Building documentation enabled for: ${DOC_TARGETS}")

    message(".....OK")


endif(WITH_DOC)
