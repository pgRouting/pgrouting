# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_conversion_tools.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgrouting_dmatrix_tools.sql
    #${CMAKE_CURRENT_SOURCE_DIR}/pgr_vidsToDMatrix_dir.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgr_fakeContraction.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/pgr_vidsToDMatrix.sql
)

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
