ADD_LIBRARY(common OBJECT 
    postgres_connection.c
    restrictions_input.c
    basePath_SSEC.cpp

    points_input.c
    matrixRows_input.c
    get_check_data.c
    edges_input.c
    
    coordinates_input.c
    arrays_input.c

    time_msg.c
    pgr_assert.cpp
    basic_vertex.cpp
    xy_vertex.cpp
    basic_edge.cpp
    )
