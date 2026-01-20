# This file is part of the pgRouting project.
# Copyright (c) 2018-2026 pgRouting developers
# License: GPL-2 See https://github.com/pgRouting/pgrouting/blob/main/LICENSE

find_package(Git)
if(GIT_FOUND AND EXISTS "${CMAKE_SOURCE_DIR}/.git")
    execute_process(
        COMMAND ${GIT_EXECUTABLE} log -1 --format=%h
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE PROJECT_GIT_HASH
        OUTPUT_STRIP_TRAILING_WHITESPACE
        )
else()
    set(PROJECT_GIT_HASH "unknown")
endif()

if (PROJECT_DEBUG)
    message(STATUS "PROJECT_GIT_HASH: ${PROJECT_GIT_HASH}")
endif()
mark_as_advanced(PROJECT_GIT_HASH)

