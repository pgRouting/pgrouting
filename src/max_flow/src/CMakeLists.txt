ADD_LIBRARY(max_flow OBJECT
        max_flow_one_to_one.c
        max_flow_many_to_many.c
        max_flow_one_to_many.c
        max_flow_many_to_one.c
        maximum_cardinality_matching.c

        max_flow_one_to_one_driver.cpp
        max_flow_many_to_many_driver.cpp
        max_flow_one_to_many_driver.cpp
        max_flow_many_to_one_driver.cpp
        maximum_cardinality_matching_driver.cpp
)
