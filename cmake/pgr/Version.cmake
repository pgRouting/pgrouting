#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# PGROUTING version variables
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------

set(PROJECT_LIB_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}")
set(PROJECT_SHORT_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}${PROJECT_VERSION_DEV}")
set(PROJECT_FULL_VERSION "v${PROJECT_VERSION}${PROJECT_VERSION_DEV}")

if (PROJECT_VERSION_DEV)
    set(PROJECT_DOC_LINK "https://docs.pgrouting.org/dev/en")
else()
    set(PROJECT_DOC_LINK "https://docs.pgrouting.org/${PROJECT_LIB_VERSION}/en")
endif()


if (PROJECT_DEBUG)
    message(STATUS "PROJECT_SHORT_VERSION: ${PROJECT_SHORT_VERSION}")
    message(STATUS "PROJECT_FULL_VERSION: ${PROJECT_FULL_VERSION}")
    message(STATUS "PROJECT_LIB_VERSION: ${PROJECT_LIB_VERSION}")
    message(STATUS "PROJECT_DOC_LINK: ${PROJECT_DOC_LINK}")

endif()
