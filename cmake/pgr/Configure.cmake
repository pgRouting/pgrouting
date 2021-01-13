
#---------------------------------------------
#  PROJECT_SRC_DIRECTORIES
#---------------------------------------------
#
# Name of the directories that have
# - C/C++ code
# - SQL code
# - documentation code
#
# structure:
#
# directory | code | sql | doc
#
# where:
#
# directory: is the name of the directory
# code: Y / N value, when "Y" C/C++ code will be looked for
# sql: Y / N value, when "Y" SQL code will be looked for
# doc: Y / N value, when "Y" Documentation code will be looked for
#----------------------
configure_file("configuration.conf" "configuration.conf")
file(STRINGS configuration.conf PROJECT_CONFIGURATION_FILE)

set(PROJECT_SRC_DIRECTORIES "")
set(PROJECT_SQL_DIRECTORIES "")
set(PROJECT_DOC_DIRECTORIES "")
foreach(line ${PROJECT_CONFIGURATION_FILE})
    string(REGEX REPLACE "^(#).*" "\\1" comment ${line})
    if("${comment}" MATCHES "#")
        continue()
    endif()
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\1" directory ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\2" has_code ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\3" has_sql ${line})
    string(REGEX REPLACE "^(.*)\\|(.*)\\|(.*)\\|(.*)" "\\4" has_doc ${line})

    string(STRIP ${directory} directory)
    string(STRIP ${has_code} has_code)
    string(STRIP ${has_sql} has_sql)
    string(STRIP ${has_doc} has_doc)


    if( ${has_code} MATCHES "Y")
        list(APPEND PROJECT_SRC_DIRECTORIES "${directory}")
    endif()
    if( ${has_sql} MATCHES "Y")
        list(APPEND PROJECT_SQL_DIRECTORIES "${directory}")
    endif()
    if( ${has_doc} MATCHES "Y")
        list(APPEND PROJECT_DOC_DIRECTORIES "${directory}")
    endif()
endforeach()

if (PROJECT_DEBUG)
    message(STATUS "PROJECT_SRC_DIRECTORIES ${PROJECT_SRC_DIRECTORIES}")
    message(STATUS "PROJECT_SQL_DIRECTORIES ${PROJECT_SQL_DIRECTORIES}")
    message(STATUS "PROJECT_DOC_DIRECTORIES ${PROJECT_DOC_DIRECTORIES}")
endif()
