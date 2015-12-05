# Append in local scope
list(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/trsp_V2.2.sql
    #${CMAKE_CURRENT_SOURCE_DIR}/routing_trsp.sql

    ${CMAKE_CURRENT_SOURCE_DIR}/routing_trsp_vias.sql
    )

# set in parent scope
set(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
