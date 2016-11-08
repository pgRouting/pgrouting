ADD_LIBRARY(astar OBJECT
    astarOneToOne.c
    astarOneToOne_driver.cpp

    astarOneToMany.c
    astarOneToMany_driver.cpp

    astarManyToOne.c
    astarManyToMany.c
    astarManyToMany_driver.cpp
    )
