# Relevant changes from 3.8 to 4.0

# WITH-DOC is not used anymore
# * The HTML documentation is build by default
# * The translated languages (en, es, zh_Hans) HTML documentation is build by default

# Documentation is not build when
# * Sphinx is not found
# * When all Sphinx formats are OFF
# * When all languages are OFF

# Output is on build/doc/_build/<format> directory

#---------------------------------------------
# Checking for prerequisites
#---------------------------------------------
find_program(SPHINX_EXECUTABLE
  NAMES sphinx-build
  DOC "Sphinx Documentation Builder (sphinx-doc.org)"
)

if (NOT SPHINX_EXECUTABLE)
  message(WARNING "Sphinx not found.
  Building without user's documentation")
  return()
endif()

set(SPHINX_FLAGS "" CACHE STRING "Flags to pass to sphinx-build")
separate_arguments(sphinx_flags UNIX_COMMAND "${SPHINX_FLAGS}")

option(BUILD_HTML "Set ON|OFF (default=OFF) to build Documentation library tree as HTML" ON)
option(BUILD_PDF "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)
option(BUILD_LOCALE "Set ON|OFF (default=OFF) to build the locale files" OFF)
option(BUILD_LINKCHECK "Set ON|OFF (default=OFF) to build Documentation library tree as LINKCHECK page" OFF)


# not quite sure how these look
option(BUILD_LATEX "Set ON|OFF (default=OFF) to build Documentation library tree as PDF" OFF)
option(BUILD_SINGLEHTML "Set ON|OFF (default=OFF) to build Documentation library tree as SINGLE HTML" OFF)
option(BUILD_MAN "Set ON|OFF (default=OFF) to build Documentation library tree as MAN page" OFF)
option(BUILD_EPUB "Set ON|OFF (default=OFF) to build Documentation library tree as EPUB page" OFF)
option(BUILD_DUMMY "Set ON|OFF (default=OFF) for debugging purposes" OFF)

# get the ON options of the Sphinx formats
foreach(opt BUILD_HTML BUILD_LATEX BUILD_MAN BUILD_SINGLEHTML BUILD_EPUB BUILD_DUMMY BUILD_LINKCHECK BUILD_LOCALE)
  if (${${opt}} OR WITH_ALL_DOC)
    string(TOLOWER ${opt} val)
    string(REPLACE "build_" "" val ${val})
    list(APPEND PROJECT_DOC_TARGETS ${val})
  endif()
endforeach()

if("${PROJECT_DOC_TARGETS}" STREQUAL "")
  message(WARNING "No documentation targets found
  Building without documentation")
  return()
endif()

add_custom_target(doc ALL COMMENT "Building documentation with Sphinx")

#---------------------------------------------
# LANGUAGES SETTINGS
#---------------------------------------------
list(APPEND PROJECT_LANGUAGES_TRANSLATED "EN" "ES" "ZH_HANS")
list(APPEND PROJECT_LANGUAGES "de" "en" "es" "ja" "ko" "sv" "zh_Hans")

foreach(lang ${PROJECT_LANGUAGES})
  string(TOUPPER ${lang} val)
  if ("${val}" IN_LIST PROJECT_LANGUAGES_TRANSLATED)
    option(${val} "Set ON|OFF (default=ON) build ${lang} Documentation" ON)
  else()
    option(${val} "Set ON|OFF (default=OFF) build ${lang} Documentation" OFF)
  endif()
endforeach()

#---------------------------------------------
# Catching the language options to be build
#---------------------------------------------
foreach(lang ${PROJECT_LANGUAGES})
  string(TOUPPER ${lang} opt)
  if (${opt})
    list(APPEND PROJECT_BUILD_LANGUAGES ${lang})
  endif()
endforeach()

if("${PROJECT_BUILD_LANGUAGES}" STREQUAL "")
  message(WARNING "No documentation languages found
  Building without documentation")
  return()
endif()

message(STATUS "PROJECT_BUILD_LANGUAGES = ${PROJECT_BUILD_LANGUAGES}")

#---------------------------------------------
# Kind of "looks"
#---------------------------------------------

if (DOC_USE_BOOTSTRAP)
    set(SPHINX_INCLUDE "import sphinx_bootstrap_theme")
    set(SPHINX_THEME "bootstrap")
    set(SPHINX_THEME_DIR "sphinx_bootstrap_theme.get_html_theme_path()")
    set(SPHINX_THEME_OPTIONS "
    'navbar_class': \"navbar\",
    'source_link_position': \"nav\",
    'bootstrap_version': \"2\",
    ")
# Modify
#   https://github.com/ryan-roemer/sphinx-bootstrap-theme/blob/master/sphinx_bootstrap_theme/bootstrap/layout.html#L67
#   adding the call to get the history of the file
    set(THEME_HISTORY "
{%- block content %}
{{ navBar() }}
<div class=\"container\">
    <div class=\"row\">
        {%- block sidebar1 %}{{ bsidebar() }}{% endblock %}
        <div class=\"body {{ bs_span_prefix }}{{ bs_content_width }} content\" role=\"main\">
            <script> document.write(get_history('{{ pagename }}','{{ language }}'));  </script>
            {% block body %}{% endblock %}
        </div>
        {% block sidebar2 %} {# possible location for sidebar #} {% endblock %}
    </div>
</div>
{%- endblock %}
    ")
else()
    set(SPHINX_INCLUDE "")
    set(SPHINX_THEME "haiku")
    set(SPHINX_THEME_DIR "['']")
    set(SPHINX_THEME_OPTIONS "")
    set(THEME_HISTORY "")
endif()

# configured documentation tools and intermediate build results
set(PGR_DOCUMENTATION_BUILD_DIR "${CMAKE_CURRENT_BINARY_DIR}/_build")
set(PGR_DOCUMENTATION_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}")

message(STATUS "PGR_DOCUMENTATION_BUILD_DIR = ${PGR_DOCUMENTATION_BUILD_DIR}")
message(STATUS "PGR_DOCUMENTATION_SOURCE_DIR = ${PGR_DOCUMENTATION_SOURCE_DIR}")


#---------------------------------------------
# Adding documentation directories
#---------------------------------------------
foreach (subdir ${PROJECT_DOC_DIRECTORIES} "src")
  add_subdirectory("${subdir}")
  if (EXISTS "${CMAKE_SOURCE_DIR}/docqueries/${subdir}")
    add_subdirectory("${CMAKE_SOURCE_DIR}/docqueries/${subdir}" "docqueries/${subdir}")
  endif()
endforeach()

add_subdirectory("images")


configure_file("../tools/testers/sampledata.sql" "${PGR_DOCUMENTATION_SOURCE_DIR}/sampledata/sampledata.sql")


add_subdirectory("_static")
add_subdirectory("_templates")

configure_file("conf.py.in" "${PGR_DOCUMENTATION_SOURCE_DIR}/conf.py" @ONLY)

foreach (format ${PROJECT_DOC_TARGETS})

message(STATUS "format=${format}")
  if ("${format}" STREQUAL "locale")

    find_program(SPHINX_INTL_EXECUTABLE
      NAMES sphinx-intl
      DOC "Sphinx Documentation Builder (sphinx-doc.org)"
    )

    if (SPHINX_INTL_EXECUTABLE)

      add_custom_target(locale
        COMMAND ${SPHINX_EXECUTABLE}
        -b gettext
        -c "${PGR_DOCUMENTATION_SOURCE_DIR}"
        "${PGR_DOCUMENTATION_SOURCE_DIR}"
        "${CMAKE_SOURCE_DIR}/locale/pot"

        COMMAND ${SPHINX_INTL_EXECUTABLE} update
        -d ${CMAKE_SOURCE_DIR}/locale
        -l en > locale_changes.txt

        COMMENT "Generating POT and PO files"
        SOURCES ${PROJECT_DOC_FILES})

    else()

      message(WARNING "sphinx-intl not found.
      Not building the pot and po locale files")

    endif()

  else()

    add_custom_target("${format}"
      COMMENT "Building ${format} documentation with Sphinx")

    configure_file(
      "${CMAKE_CURRENT_SOURCE_DIR}/forward.html"
      "${PGR_DOCUMENTATION_BUILD_DIR}/${format}/index.html")

    foreach (lang ${PROJECT_BUILD_LANGUAGES})
      add_custom_target(
        "${format}-${lang}"
        COMMAND ${SPHINX_EXECUTABLE}
        -c ${CMAKE_CURRENT_BINARY_DIR}
        -d ${CMAKE_CURRENT_BINARY_DIR}/doctrees
        -b ${format}
        -D language='${lang}'
        ${sphinx_flags}
        "${PGR_DOCUMENTATION_SOURCE_DIR}"
        "${PGR_DOCUMENTATION_BUILD_DIR}/${format}/${lang}"

        COMMENT "Generating ${format}/${lang} documentation"
        SOURCES ${PROJECT_DOC_FILES}  ${PROJECT_IMG_FILES}
        DEPENDS ${PROJECT_DOC_FILES}  ${PROJECT_IMG_FILES}
      )
      add_dependencies(${format} "${format}-${lang}")

    endforeach()
    add_dependencies(doc "${format}")

  endif()
endforeach()
