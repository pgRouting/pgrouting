ADD_LIBRARY(ordering OBJECT
    kingOrdering.c
    cuthillMckeeOrdering.c
    cuthillMckeeOrdering_driver.cpp
    sloanOrdering.c
    topologicalSort.c
    topologicalSort_driver.cpp

    ordering_driver.cpp
    ordering_process.cpp
)
