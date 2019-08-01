ADD_LIBRARY(version OBJECT
        version.c
        _version.c
        )
configure_file(_version.h.in ${CMAKE_CURRENT_LIST_DIR}/_version.h)
