#
# Find Sphinx
# Find Sphinx executable to build documentation
# Source: https://ericscottbarr.com/blog/2012/03/sphinx-and-cmake-beautiful-documentation-for-c-projects/
#
# Daniel Kastl 03/2013
#

#---------------------------------------------
# Kind of sphinx output
#---------------------------------------------

option(WITH_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

option(LOCALE
    "Set ON|OFF (default=OFF) to build ithe latest pot/po files" OFF)

option(WITH_ALL_DOC
    "Set ON|OFF (default=OFF) to build Documentation library tree" OFF)

#option(PDF
#    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(BUILD_LATEX
    "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)

option(SINGLEHTML
    "Set ON|OFF (default=OFF) to build Documentation library tree as SINGLE HTML" OFF)

option(BUILD_HTML
    "Set ON|OFF (default=OFF) to build Documentation library tree as HTML" ON)

option(BUILD_MAN
    "Set ON|OFF (default=OFF) to build Documentation library tree as MAN page" OFF)

option(LINKCHECK
    "Set ON|OFF (default=OFF) to build Documentation library tree as LINKCHECK page" OFF)

option(EPUB
    "Set ON|OFF (default=OFF) to build Documentation library tree as EPUB page" OFF)

option(DUMMY
    "Set ON|OFF (default=OFF) for debugging purposes" OFF)

if (LOCALE)
    set(WITH_DOC ON)
endif()

if (NOT WITH_DOC)
    return()
endif()

foreach(opt BUILD_HTML BUILD_LATEX BUILD_MAN SINGLEHTML EPUB DUMMY LINKCHECK)
    if (${${opt}} OR WITH_ALL_DOC)
        string(TOLOWER ${opt} val)
        string(REPLACE "build_" "" val ${val})
        list(APPEND PGROUTING_DOC_TARGETS ${val})
        set(WITH_DOC ON)
    endif()
endforeach()



message(STATUS "PGROUTING_DOC_TARGETS = ${PGROUTING_DOC_TARGETS}")

#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

# LANGUAGES SETINGS

#---------------------------------------------
#---------------------------------------------
#---------------------------------------------

#---------------------------------------------
# Available Languages
# English is always built
#---------------------------------------------
set(PGROUTING_SUPPORTED_LANGUAGES  "de"   "es"  "fr"  "it"  "ja"  "ru")
set(PGROUTING_ENGLISH "en")

#---------------------------------------------
# Language options
#---------------------------------------------
option(WITH_ALL_LANG
    "Set ON|OFF (default=OFF) to build all the supported languages:
    Main Language will always be built:
    ${PGROUTING_ENGLISH}
    Optional languages:
    ${PGROUTING_SUPPORTED_LANGUAGES}

    When OFF and want to build specific optional language, use the language code in capital letters:
    -D ES=ON
    "
    OFF
    )

foreach(lang ${PGROUTING_SUPPORTED_LANGUAGES})
    string(TOUPPER ${lang} val)
    option(${val}
        "Set ON|OFF (default=OFF) build ${lang} Documentation" OFF)
endforeach()

#---------------------------------------------
# Catching the language options to be build
#---------------------------------------------
foreach(lang ${PGROUTING_SUPPORTED_LANGUAGES})
    string(TOUPPER ${lang} opt)
    if (${opt} OR WITH_ALL_LANG)
        set (langFound  "1")
        list(APPEND PGROUTING_BUILD_LANGUAGES ${lang})
    endif()
endforeach()


#---------------------------------------------
# All languages po files are to be generated
#---------------------------------------------

set (SPHINXINTL_LANGUAGE  ${PGROUTING_ENGLISH})
foreach(lang ${PGROUTING_BUILD_LANGUAGES})
    set(SPHINXINTL_LANGUAGE "${SPHINXINTL_LANGUAGE},${lang}")
endforeach()


list(APPEND PGROUTING_BUILD_LANGUAGES  ${PGROUTING_ENGLISH})
message(STATUS "PGROUTING_BUILD_LANGUAGES = ${PGROUTING_BUILD_LANGUAGES}")
message(STATUS "SPHINXINTL_LANGUAGE = ${SPHINXINTL_LANGUAGE}")


if(NOT WITH_DOC)
    return()
endif()


#---------------------------------------------
# Checking for prerequisites
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




# configured documentation tools and intermediate build results
# TODO for 3.0 Use _build
# set(PGR_DOCUMENTATION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
set(PGR_DOCUMENTATION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(PGR_DOCUMENTATION_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

message(STATUS "PGR_DOCUMENTATION_BUILD_DIR = ${PGR_DOCUMENTATION_BUILD_DIR}")


#---------------------------------------------
# Adding documentation directories
#---------------------------------------------
foreach (subdir ${PGROUTING_DOC_DIRECTORIES} "src")
    add_subdirectory("${subdir}")
    add_subdirectory("../docqueries/${subdir}" "build/doc/${subdir}")
endforeach()

add_subdirectory("images")


configure_file("../tools/testers/sampledata.sql" "${PGR_DOCUMENTATION_SOURCE_DIR}/sampledata/sampledata.sql")


#TODO ???
#add_subdirectory("static")
#add_subdirectory("themes")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/_static" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/_themes" DESTINATION "${PGR_DOCUMENTATION_SOURCE_DIR}")


configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/conf.py.in"
    "${PGR_DOCUMENTATION_SOURCE_DIR}/conf.py"
    @ONLY)

# Add documentation to targets


#if (LATEX)
if (FALSE)
    find_package(LATEX)
    if (NOT LATEX_COMPILER)
        message("Couldn't find Latex, can't build latex docs using Sphinx")
    else()
        message(STATUS "LATEX_COMPILER = ${LATEX_COMPILER}")
    endif()
endif()

set(ENV{SPHINXINTL_LANGUAGE} ${SPHINXINTL_LANGUAGE})

if (LOCALE)
    add_custom_target(locale
        COMMAND ${SPHINX_EXECUTABLE}
        -b gettext
        -c "${PGR_DOCUMENTATION_SOURCE_DIR}"
        "${PGR_DOCUMENTATION_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/locale/pot"

        COMMAND sphinx-intl update -d ${CMAKE_SOURCE_DIR}/locale -l en
    #COMMAND sphinx-intl update -p ${CMAKE_SOURCE_DIR}/locale/pot -d ${CMAKE_SOURCE_DIR}/locale --language=${SPHINXINTL_LANGUAGE}
    #COMMAND sphinx-intl update-txconfig-resources --locale-dir ${CMAKE_SOURCE_DIR}/locale --pot-dir ${CMAKE_SOURCE_DIR}/locale/pot --transifex-project-name pgrouting


    COMMENT "Generating POT files ..."
    SOURCES ${PGROUTING_DOC_FILES}
    )
    return()
endif()


# The doc target will build all documentation targets.
add_custom_target(doc
    COMMENT "Building documentation with Sphinx")


foreach (target ${PGROUTING_DOC_TARGETS})
    add_custom_target("${target}"
        COMMENT "Building ${target} documentation with Sphinx")

    configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
        "${PGR_DOCUMENTATION_BUILD_DIR}/${target}/index.html")

    foreach (lang ${PGROUTING_BUILD_LANGUAGES})
        add_custom_target(
            "${target}-${lang}"
            COMMAND ${SPHINX_EXECUTABLE}
            #-q  # Enable for quiet mode
            #-Q  # Enable for even more quiet mode
            -v   # increase verbosity
            -W
            --keep-going
            #-j auto
            #-E  # don't use a saved environment, always read all file
            #-a  # write all files; default is to only write new and changed files
            -b ${target}
            -D language='${lang}'
            -c "${PGR_DOCUMENTATION_SOURCE_DIR}"
            "${PGR_DOCUMENTATION_SOURCE_DIR}"
            "${PGR_DOCUMENTATION_BUILD_DIR}/${target}/${lang}"

            COMMENT "Generating ${target}/${lang} documentation ..."
            SOURCES ${PGROUTING_DOC_FILES}  ${PGROUTING_IMG_FILES}
            DEPENDS ${PGROUTING_DOC_FILES}  ${PGROUTING_IMG_FILES}
            )
        add_dependencies(${target} "${target}-${lang}")

    endforeach()
    add_dependencies(doc "${target}")
endforeach()



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

