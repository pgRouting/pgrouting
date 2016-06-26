# Append in local scope
LIST(APPEND PACKAGE_SQL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_boykov_kolmogorov.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_edmonds_karp.sql
    ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_push_relabel.sql
        ${CMAKE_CURRENT_SOURCE_DIR}/max_flow_push_relabel_multi.sql

        )

# set in parent scope
SET(PACKAGE_SQL_FILES "${PACKAGE_SQL_FILES}" PARENT_SCOPE)
