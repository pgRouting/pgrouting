ADD_LIBRARY(common OBJECT
    postgres_connection.c
    e_report.c
    basePath_SSEC.cpp

    check_parameters.c

    time_msg.c
    assert.cpp
    identifier.cpp

    basic_vertex.cpp
    xy_vertex.cpp
    ch_vertex.cpp
    ch_edge.cpp
    basic_edge.cpp
    alloc.cpp
    )
