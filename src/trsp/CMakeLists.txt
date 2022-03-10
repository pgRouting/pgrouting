add_library(trsp OBJECT
    # keeping these for backwards compatibility
    trsp.c
    trsp_core.cpp
    GraphDefinition.cpp


    new_trsp.c
    edgeInfo.cpp
    pgr_trspHandler.cpp
    trsp_driver.cpp
    trsp_withPoints.c
    trsp_withPoints_driver.cpp

    trspVia.c
    trspVia_driver.cpp

    trspVia_withPoints.c
    trspVia_withPoints_driver.cpp
    )
