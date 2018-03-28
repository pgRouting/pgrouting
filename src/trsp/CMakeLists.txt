add_library(trsp OBJECT 
    # keeping these for backwards compatibility
    trsp.c
    trsp_core.cpp
    GraphDefinition.cpp
    
    
    new_trsp.c
    edgeInfo.cpp
    pgr_trspHandler.cpp
    rule.cpp
    trsp_driver.cpp

    )
