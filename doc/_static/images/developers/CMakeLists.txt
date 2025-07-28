SET(LOCAL_FILES
    disconnectEdgeDirected.png
    disconnectEdgeUndirected.png
    disconnectVertexDirected.png
    disconnectVertexUndirected.png
)

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f})
    list(APPEND LOCAL_IMG_FILES "${CMAKE_CURRENT_BINAYR_DIR}/${f}")
endforeach()

set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)
