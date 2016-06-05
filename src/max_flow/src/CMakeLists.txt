ADD_LIBRARY(max_flow OBJECT
        max_flow_push_relabel.c
        max_flow_edmonds_karp.c

        max_flow_push_relabel_driver.cpp
        max_flow_edmonds_karp_driver.cpp)
