# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pickDeliverEuclidean.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pickDeliver.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/reginabook.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
