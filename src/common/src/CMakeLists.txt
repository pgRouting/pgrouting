ADD_LIBRARY(common OBJECT 
    postgres_connection.c
    restrictions_input.c
    basePath_SSEC.cpp
    points_input.c
    get_check_data.c
    edges_input.c
    arrays_input.c

    pgr_assert.cpp
    )
