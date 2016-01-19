# Append in local scope
# PACKAGE_SQL_FILES is a list to which the paths to sql files are being added 
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/myastar.sql
)
# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
