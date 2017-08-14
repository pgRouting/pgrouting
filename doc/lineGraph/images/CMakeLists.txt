SET(LOCAL_FILES
    128px-Line_graph_construction_4.svg.png
    135px-Line_graph_construction_1.svg.png
    135px-Line_graph_construction_2.svg.png
    135px-Line_graph_construction_3.svg.png

    Graph1.png
    Graph2.png

    Line-graph-with-edge-cost.png
    Line-graph-with-node-cost.png
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}" COPYONLY)
    list(APPEND LOCAL_IMG_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}")
endforeach()

set(PgRouting_IMG_FILES ${PgRouting_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)
