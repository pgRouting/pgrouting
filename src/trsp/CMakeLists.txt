add_library(trsp OBJECT
    # keeping these for backwards compatibility
    trsp_deprecated.c
    trsp_deprecated_driver.cpp
    GraphDefinition.cpp
    edgeInfo.cpp
    trspHandler.cpp

    trsp.c
    trsp_driver.cpp

    trsp_withPoints.c
    trsp_withPoints_driver.cpp

    trspVia.c
    trspVia_driver.cpp

    trspVia_withPoints.c
    trspVia_withPoints_driver.cpp
    )
