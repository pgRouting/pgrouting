ADD_LIBRARY(astar OBJECT
    astarOneToOne.c
    astarOneToOne_driver.cpp

    #for version part of astar functionality
    #astarOneToMany.c
    #astarOneToMany_driver.cpp

    #astarManyToOne.c
    #astarManyToMany.c
    #astarManyToMany_driver.cpp
    )
