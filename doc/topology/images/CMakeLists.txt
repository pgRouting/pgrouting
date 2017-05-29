
SET(LOCAL_FILES
    after_node_net.png
    before_node_net.png
    #Fig3-reverseCost.png
    #Fig5-reverseCostUndirected.png
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}" COPYONLY)
    list(APPEND LOCAL_IMG_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}")
endforeach()

set(PgRouting_IMG_FILES ${PgRouting_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)

