foreach (f ${MINORS})
    configure_file(${PROJECT_NAME_LOWER}--${f}.sig ${PROJECT_NAME_LOWER}--${f}.sig COPYONLY)
    list(APPEND SIGNATURE_FILES ${PROJECT_NAME_LOWER}--${f}.sig)
endforeach()

