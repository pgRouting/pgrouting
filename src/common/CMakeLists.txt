ADD_LIBRARY(common OBJECT
    postgres_connection.c
    e_report.c
    #restrict_input.c
    basePath_SSEC.cpp

    points_input.c
    matrixRows_input.c
    get_check_data.c
    edges_input.c
    orders_input.c
    orders_input.c
    vehicles_input.c
    pgr_point_input.c

    restrictions_input.c

    coordinates_input.c
    arrays_input.c
    delauny_input.c

    check_parameters.c

    time_msg.c
    pgr_assert.cpp
    identifier.cpp

    basic_vertex.cpp
    xy_vertex.cpp
    ch_vertex.cpp
    ch_edge.cpp
    basic_edge.cpp
    pgr_alloc.cpp
    )
