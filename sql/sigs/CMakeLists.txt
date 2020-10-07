SET(OLD_SIGNATURES
    3.1.0

    3.0.3
    3.0.2
    3.0.1
    3.0.0

    2.6.3
    2.6.2
    2.6.1
    2.6.0
    )

foreach (f ${OLD_SIGNATURES})
    configure_file(pgrouting--${f}.sig pgrouting--${f}.sig COPYONLY)
    list(APPEND SIGNATURE_FILES pgrouting--${f}.sig)
endforeach()
configure_file(pgrouting--${PGROUTING_VERSION}.sig pgrouting--${PGROUTING_VERSION}.sig COPYONLY)

set(OLD_SIGNATURES ${OLD_SIGNATURES} PARENT_SCOPE)
