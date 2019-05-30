
SET(LOCAL_FILES
    cc_sampledata.png
    scc_sampledata.png
    bcc_sampledata.png
    ap_sampledata.png
    bridge_sampledata.png
    )

foreach (f ${LOCAL_FILES})
    configure_file(${f} "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}" COPYONLY)
    list(APPEND LOCAL_IMG_FILES  "${PGR_DOCUMENTATION_SOURCE_DIR}/images/${f}")
endforeach()

set(PGROUTING_IMG_FILES ${PGROUTING_IMG_FILES} ${LOCAL_IMG_FILES} PARENT_SCOPE)

