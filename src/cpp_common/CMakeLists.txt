ADD_LIBRARY(cpp_common OBJECT
    Dmatrix.cpp
    compPaths.cpp
    rule.cpp
    bpoint.cpp
    messages.cpp
    combinations.cpp

    get_check_data.cpp
    pgdata_fetchers.cpp
    pgdata_getters.cpp

    # for old trsp code
    trsp_pgfetch.cpp
    # linked as C, but compiled as C++
    trsp_pgget.cpp
    )
