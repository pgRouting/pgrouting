ADD_LIBRARY(ordering OBJECT
    kingOrdering.c
    cuthillMckeeOrdering.c
    sloanOrdering.c
    topologicalSort.c

    ordering_driver.cpp
    ordering_process.cpp

    cuthillMckeeOrdering.cpp
    kingOrdering.cpp
    sloanOrdering.cpp
    topologicalSort.cpp
)
