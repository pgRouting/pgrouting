ADD_LIBRARY(cpp_common OBJECT
    Dmatrix.cpp
    compPaths.cpp
    rule.cpp
    bpoint.cpp
    pgr_messages.cpp
    combinations.cpp

    get_check_data.cpp
    pgdata_fetchers.cpp
    trsp_pgfetch.cpp
    # linked as C, but compiled as C++
    pgdata_getters.cpp
    )
