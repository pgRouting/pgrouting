# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

option(WITH_DOC
     "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(BUILD_LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(BUILD_HTML
    "Set ON|OFF (default=ON) to build Documentation library tree as HTML" ON)

option(BUILD_MAN
    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" OFF)


if(WITH_DOC)
    message("PROCESSING DOCUMENTATION------------")

    find_package(Sphinx REQUIRED)

    set(SPHINX_THEME "haiku")
    #set(SPHINX_THEME_DIR "_themes")

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
        set(PGR_DOCUMENTATION_BUILD_DIR "${PGROUTING_BINARY_DIR}/doc/_build")
        set(PGR_DOCUMENTATION_SOURCE_DIR "${PGROUTING_BINARY_DIR}/doc")

        message(STATUS "PGR_DOCUMENTATION_BUILD_DIR = ${PGR_DOCUMENTATION_BUILD_DIR}")

        # ---------- TO FLATTEN THE DOCUMENTATION ---------

        file(GLOB PGR_DOCUMENTATION_SOURCES "src/*")
        foreach (file ${PGR_DOCUMENTATION_SOURCES})
            message("${file}")
            file(COPY "${file}" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}")
        endforeach()

        file(GLOB  PGR_DOCUMENTATION_SOURCES "../src/*/doc/*")
        foreach (file ${PGR_DOCUMENTATION_SOURCES})
            message("${file}")
            file(COPY "${file}" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}")
        endforeach()

        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/index.rst" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}")
        file(COPY "../tools/testers/sampledata.sql" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}/sampledata/")


        #message(STATUS "PGROUTING_BINARY_DIR = ${PGROUTING_BINARY_DIR}")

        # Sphinx cache with pickled ReST documents
        set(SPHINX_CACHE_DIR "${PGROUTING_BINARY_DIR}/doc/_doctrees")

        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${PGR_DOCUMENTATION_BUILD_DIR}")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${PGR_DOCUMENTATION_BUILD_DIR}")
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/CNAME" DESTINATION "${PGROUTING_BINARY_DIR}")

        #configure_file(
        #    "${CMAKE_CURRENT_SOURCE_DIR}/index.html.in"
        #    "${PGROUTING_BINARY_DIR}/doc/html/index.html")

        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
            "${PGROUTING_BINARY_DIR}/doc/html/en/index.html")

        # Add documentation to targets
        set(DOC_TARGETS html)

        #option(BUILD_MAN "Create a target for building man pages." ON)

        if (BUILD_MAN)
            if (SPHINX_VERSION VERSION_LESS 1.0)
                message(WARNING "Sphinx version 1.0 > is required to build man pages. You have v${SPHINX_VERSION}.")
            else()
                list(APPEND DOC_TARGETS man)
            endif()
        endif()

        #option(BUILD_LATEX "Create a target for building latex docs (to create PDF)." ON)

        if (BUILD_LATEX)
            find_package(LATEX)

            if (NOT LATEX_COMPILER)
                message("Couldn't find Latex, can't build latex docs using Sphinx")
            else()
                message(STATUS "LATEX_COMPILER = ${LATEX_COMPILER}")
                list(APPEND DOC_TARGETS latex)
            endif()
        endif()

        # The doc target will build all documentation targets.
        add_custom_target(doc #ALL
            COMMENT "Building documentation with Sphinx")
        # add_custom_target(doc)

        # Localization output directory
        #set(SPHINX_I18N_DIR "${CMAKE_CURRENT_SOURCE_DIR}/i18n/pot")

        configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
            "${PGR_DOCUMENTATION_BUILD_DIR}/conf.py"
            @ONLY)

        foreach (DOC_TARGET ${DOC_TARGETS})
            add_custom_command(
                TARGET doc POST_BUILD
                COMMAND ${SPHINX_EXECUTABLE} 
                # -q  # Enable for quiet mode 
                # -Q  # Enable for even more quiet mode
                # -v  # increase verbosity
                -b ${DOC_TARGET}
                -E  # don't use a saved environment, always read all file
                -a  # write all files; default is to only write new and changed files
                -d "${SPHINX_CACHE_DIR}" 
                -c "${PGR_DOCUMENTATION_BUILD_DIR}" 
                "${PGR_DOCUMENTATION_SOURCE_DIR}" 
                "${PGROUTING_BINARY_DIR}/doc/${DOC_TARGET}/en"
                COMMENT "Generating ${DOC_TARGET} documentation ...")
        endforeach()

        if(BUILD_LATEX)
            if(LATEX_COMPILER)
                add_custom_command(
                    TARGET doc POST_BUILD
                    #fixes: Underfull \hbox (badness 10000) in paragraph
                    #perl -pi -e 's/warning/warning /' pgRoutingDocumentation.tex
                    COMMAND pdflatex 
                    -interaction=nonstopmode 
                    "pgRoutingDocumentation.tex" > /dev/null 2>&1
                    WORKING_DIRECTORY "${PGROUTING_BINARY_DIR}/doc/latex/en"
                    COMMENT "Converting Latex to PDF format")

                list(APPEND DOC_TARGETS pdf)
            endif()
        endif()

        message("-- Building documentation enabled for: ${DOC_TARGETS}")
    endif()

    message("END PROCESSING DOCUMENTATION------------")
endif(WITH_DOC)
