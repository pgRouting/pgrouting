ADD_LIBRARY(pickDeliver OBJECT
    base_node.cpp
    dnode.cpp
    node.cpp
    tw_node.cpp
    vehicle_node.cpp

    order.cpp
    pd_orders.cpp
    fleet.cpp
    vehicle.cpp
    pd_problem.cpp
    vehicle_pickDeliver.cpp
    book_keeping.cpp

    solution.cpp
    initial_solution.cpp
    optimize.cpp

    pgr_messages.cpp
    pgr_pickDeliver.cpp


    pickDeliverEuclidean_driver.cpp
    pickDeliver_driver.cpp
    pickDeliverEuclidean.c
    pickDeliver.c

    )
