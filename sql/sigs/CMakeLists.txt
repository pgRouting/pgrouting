foreach (f ${MINORS})
    configure_file(pgrouting--${f}.sig pgrouting--${f}.sig COPYONLY)
    list(APPEND SIGNATURE_FILES pgrouting--${f}.sig)
endforeach()

