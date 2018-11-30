SET(OLD_SIGNATURES
    2.6.1
    2.6.0

    2.5.5
    2.5.4
    2.5.3
    2.5.2
    2.5.1
    2.5.0

    2.4.2
    2.4.1
    2.4.0

    2.3.2
    2.3.1
    2.3.0

    2.2.4
    2.2.3
    2.2.2
    2.2.1

    2.2.0
    2.1.0

    2.0.1
    2.0.0
    )

foreach (f ${OLD_SIGNATURES})
    configure_file(pgrouting--${f}.sig pgrouting--${f}.sig COPYONLY)
    list(APPEND SIGNATURE_FILES pgrouting--${f}.sig)
endforeach()
configure_file(pgrouting--${PGROUTING_VERSION}.sig pgrouting--${PGROUTING_VERSION}.sig COPYONLY)

set(OLD_SIGNATURES ${OLD_SIGNATURES} PARENT_SCOPE)
