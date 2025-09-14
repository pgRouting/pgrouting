SET(LOCAL_FILES
    ccbysa.png
    pgrouting.png
    favicon.ico
)

foreach (f ${LOCAL_FILES})
    configure_file(${f} ${f} COPYONLY)
    list(APPEND LOCAL_IMG_FILES "${CMAKE_CURRENT_BINARY_DIR}/${f}")
endforeach()

add_subdirectory(developers)
set(PROJECT_IMG_FILES ${PROJECT_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)
