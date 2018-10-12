
option(SET_VERSION_BRANCH
    "Set ON|OFF (default=OFF) to build the version file" OFF)


if ((CMAKE_BUILD_TYPE == "Debug") AND (EXISTS "${CMAKE_SOURCE_DIR}/.git"))

    # Get the current working branch
    execute_process(
        COMMAND git rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PGROUTING_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    # Get the latest abbreviated commit hash of the working branch
    execute_process(
        COMMAND git log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PGROUTING_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    # Get the number of commits of the working branch
    execute_process(
        COMMAND git rev-list HEAD --count
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PGROUTING_GIT_COMMITS
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )

    if (SET_VERSION_BRANCH)
        file(WRITE "${PGROUTING_SOURCE_DIR}/VERSION" "${PGROUTING_GIT_COMMITS}-${PGROUTING_GIT_HASH} ${SET_VERSION_BRANCH}")
    endif()

else()

    #---------------------------------------------
    # Taking info from file
    #---------------------------------------------
    file(STRINGS "${PGROUTING_SOURCE_DIR}/VERSION" PGROUTING_BUILD_STRING)

    set(PGROUTING_GIT_HASH "")
    string(REGEX REPLACE "^([0-9]+)-([0-9a-f]+) (.*)$" "\\2" PGROUTING_GIT_HASH ${PGROUTING_BUILD_STRING})
    string(REGEX REPLACE "^([0-9]+)-([0-9a-f]+) (.*)$" "\\3" PGROUTING_GIT_BRANCH ${PGROUTING_BUILD_STRING})

endif()



add_definitions("-DPGROUTING_GIT_HASH=${PGROUTING_GIT_HASH}")
add_definitions("-DPGROUTING_GIT_BRANCH=${PGROUTING_GIT_BRANCH}")

if (PGROUTING_DEBUG)
    message(STATUS "PGROUTING_GIT_BRANCH: ${PGROUTING_GIT_BRANCH}")
    message(STATUS "PGROUTING_GIT_HASH: ${PGROUTING_GIT_HASH}")
endif()

