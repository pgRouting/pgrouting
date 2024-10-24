ADD_LIBRARY(pickDeliver OBJECT
    pd_orders.cpp
    fleet.cpp
    pickDeliver.cpp
    dnode.cpp
    vehicle_pickDeliver.cpp
    solution.cpp
    optimize.cpp

    tw_node.cpp
    vehicle_node.cpp

    order.cpp

    initial_solution.cpp
    vehicle.cpp

    pickDeliverEuclidean_driver.cpp
    pickDeliver_driver.cpp
    pickDeliverEuclidean.c
    pickDeliver.c

    pd_problem.cpp
    )
