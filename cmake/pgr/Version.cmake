#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# PGROUTING version variables
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

set(PGROUTING_LIB_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}")
set(PGROUTING_SHORT_VERSION "${PGROUTING_VERSION_MAJOR}.${PGROUTING_VERSION_MINOR}${PGROUTING_VERSION_DEV}")
set(PGROUTING_FULL_VERSION "v${PGROUTING_VERSION}${PGROUTING_VERSION_DEV}")

if (PGROUTING_VERSION_DEV)
    set(PGROUTING_DOC_LINK "https://docs.pgrouting.org/dev/en")
else()
    set(PGROUTING_DOC_LINK "https://docs.pgrouting.org/${PGROUTING_LIB_VERSION}/en")
endif()


if (PGROUTING_DEBUG)
    message(STATUS "PGROUTING_VERSION: ${PGROUTING_VERSION}")
    message(STATUS "PGROUTING_VERSION_MAJOR: ${PGROUTING_VERSION_MAJOR}")
    message(STATUS "PGROUTING_VERSION_MINOR: ${PGROUTING_VERSION_MINOR}")
    message(STATUS "PGROUTING_VERSION_PATCH: ${PGROUTING_VERSION_PATCH}")
    message(STATUS "PGROUTING_VERSION_TWEAK: ${PGROUTING_VERSION_TWEAK}")
    message(STATUS "PGROUTING_VERSION_DEV: ${PGROUTING_VERSION_DEV}")

    message(STATUS "PGROUTING_SHORT_VERSION: ${PGROUTING_SHORT_VERSION}")
    message(STATUS "PGROUTING_FULL_VERSION: ${PGROUTING_FULL_VERSION}")

    message(STATUS "PGROUTING_LIB_VERSION: ${PGROUTING_LIB_VERSION}")
    message(STATUS "PGROUTING_DOC_LINK: ${PGROUTING_DOC_LINK}")

endif()
