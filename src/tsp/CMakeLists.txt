set(PACKAGE_SQL_FILES "")
add_subdirectory(sql)
set(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
subdirs(doc src test)
