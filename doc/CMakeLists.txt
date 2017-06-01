# 
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: http://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
# 
# Daniel Kastl 03/2013
#

#---------------------------------------------
# Kind of sphinx output
#---------------------------------------------

option(WITH_ALL_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(PDF
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(SINGLEHTML
    "Set ON|OFF (default=ON) to build Documentation library tree as SINGLE HTML" OFF)

option(HTML
    "Set ON|OFF (default=ON) to build Documentation library tree as HTML" OFF)

#option(MAN
#    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" OFF)

option(EPUB
    "Set ON|OFF (default=ON) to build Documentation library tree as EPUB page" OFF)

option(DUMMY
    "Set ON|OFF (default=ON) for debugging purposes" OFF)

option(LOCALE
    "Set ON|OFF (default=OFF) to build the locale files of all languages.
    All other options are ignored
    All supported languages are considered" OFF
    )

if (NOT LOCALE)
    foreach(opt HTML LATEX PDF SINGLEHTML EPUB DUMMY)
        if (${${opt}} OR WITH_ALL_DOC)
            string(TOLOWER ${opt} val)
            list(APPEND PgRouting_DOC_TARGETS ${val})
            set(WITH_DOC ON)
        endif()
    endforeach()
else()
    set(WITH_DOC ON)
endif()

if(NOT WITH_DOC)
    return()
endif()

message(STATUS "PgRouting_DOC_TARGETS = ${PgRouting_DOC_TARGETS}")

#---------------------------------------------
# Checking for prerwquisites
#---------------------------------------------
find_package(Sphinx ${SPHINX_MINIMUM_VERSION} REQUIRED)

if (NOT SPHINX_FOUND)
    message(FATAL_ERROR "Sphinx not found.
    Cannot generate documentation!
    Install Sphinx or build without documentation")
endif()

if (SPHINX_VERSION VERSION_LESS 1.0)
    message(WARNING "Your Sphinx version is too old!
    This project requires Sphinx v${SPHINX_MINIMUM_VERSION} or above to produce
    proper documentation (you have v${SPHINX_VERSION}).
    You will get output but it will have errors.")
endif()

#pip install rst2pdf



#---------------------------------------------
# Kind of "looks"
#---------------------------------------------
option(DOC_USE_BOOTSTRAP
    "Set ON|OFF (default=ON) to build Documentation library tree as MAN page" OFF)

if (DOC_USE_BOOTSTRAP)
    set(SPHINX_INCLUDE "import sphinx_bootstrap_theme")
    set(SPHINX_THEME "bootstrap")
    set(SPHINX_THEME_DIR "sphinx_bootstrap_theme.get_html_theme_path()")
    set(SPHINX_THEME_OPTIONS "'source_link_position': \"none\",")
else()
    set(SPHINX_INCLUDE "")
    set(SPHINX_THEME "haiku")
    set(SPHINX_THEME_DIR "['']")
    set(SPHINX_THEME_OPTIONS "")
endif()



#TODO make variables for the languages
set(PGR_DOCUMENTATION_LANGUAGES "en" "es")




# configured documentation tools and intermediate build results
set(PGR_DOCUMENTATION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
set(PGR_DOCUMENTATION_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

message(STATUS "PGR_DOCUMENTATION_BUILD_DIR = ${PGR_DOCUMENTATION_BUILD_DIR}")


#---------------------------------------------
# Adding documentation directories
#---------------------------------------------
add_subdirectory("src")
add_subdirectory("images")
add_subdirectory("queries")

foreach (subdir ${PgRouting_DOC_DIRECTORIES})
    add_subdirectory(${subdir})
endforeach()

configure_file("../tools/testers/sampledata.sql" "${PGR_DOCUMENTATION_SOURCE_DIR}/sampledata/sampledata.sql")


#TODO ???
#add_subdirectory("static")
#add_subdirectory("themes")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/static" DESTINATION "${PGR_DOCUMENTATION_BUILD_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/themes" DESTINATION "${PGR_DOCUMENTATION_BUILD_DIR}")


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
    "${PGR_DOCUMENTATION_BUILD_DIR}/conf.py"
    @ONLY)

# Add documentation to targets


if (LATEX)
    find_package(LATEX)
    if (NOT LATEX_COMPILER)
        message("Couldn't find Latex, can't build latex docs using Sphinx")
    else()
        message(STATUS "LATEX_COMPILER = ${LATEX_COMPILER}")
    endif()
endif()

# The doc target will build all documentation targets.
add_custom_target(doc 
    COMMENT "Building documentation with Sphinx")


foreach (target ${PgRouting_DOC_TARGETS})
    add_custom_target("${target}"
            COMMENT "Building ${target} documentation with Sphinx")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
        "${PGR_DOCUMENTATION_BUILD_DIR}/${target}/index.html")

    foreach (lang ${PGR_DOCUMENTATION_LANGUAGES})
        add_custom_target("${target}-${lang}"
            COMMENT "Building ${target}/${lang} documentation with Sphinx")

        add_custom_command(
            TARGET "${target}-${lang}" POST_BUILD
            COMMAND ${SPHINX_EXECUTABLE} 
            #-q  # Enable for quiet mode 
            #-Q  # Enable for even more quiet mode
            #-v  # increase verbosity
            #-E  # don't use a saved environment, always read all file
            #-a  # write all files; default is to only write new and changed files
            -b ${target}
            -D language='${lang}'
            -c "${PGR_DOCUMENTATION_BUILD_DIR}" 
            "${PGR_DOCUMENTATION_SOURCE_DIR}" 
            "${PGR_DOCUMENTATION_BUILD_DIR}/${target}/${lang}"
            COMMENT "Generating ${target}/${lang} documentation ...")
        add_dependencies(${target} "${target}-${lang}")

    endforeach()
    add_dependencies(doc "${target}")
endforeach()


if (LOCALE)
    add_custom_target(locale 
        COMMENT "Building documentation with Sphinx")
    add_custom_command(
        TARGET locale POST_BUILD

        COMMAND ${SPHINX_EXECUTABLE} 
        -b gettext
        -c "${PGR_DOCUMENTATION_BUILD_DIR}" 
        "${PGR_DOCUMENTATION_SOURCE_DIR}" 
        "${CMAKE_SOURCE_DIR}/locale/pot" 

        COMMAND sphinx-intl update -p ${CMAKE_SOURCE_DIR}/locale/pot -d ${CMAKE_SOURCE_DIR}/locale  -l es -l en

        # COMMAND tx pull -r pgrouting.pgr_createVerticesTable -f -l es


        COMMENT "Generating POT files ...")
    add_dependencies(doc locale)
endif()




if(FALSE)
    # AND ${target} MATCHES "latex" AND LATEX_COMPILER)
    add_custom_target(pdf-${lang}
        COMMENT "Building documentation with Sphinx")

    add_custom_command(
        TARGET pdf-${lang} POST_BUILD
        #fixes: Underfull \hbox (badness 10000) in paragraph
        #perl -pi -e 's/warning/warning /' pgRoutingDocumentation.tex
        COMMAND pdflatex -interaction=nonstopmode "pgRoutingDocumentation.tex" > /dev/null 2>&1
        WORKING_DIRECTORY "${PGR_DOCUMENTATION_BUILD_DIR}/pdf/${lang}"
        DEPENDS ${target}-${lang}
        COMMENT "Converting Latex to PDF format")

endif()

