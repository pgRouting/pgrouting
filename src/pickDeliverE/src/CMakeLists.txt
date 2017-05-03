ADD_LIBRARY(pickDeliverE OBJECT
    base_node.cpp
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

    pgr_pickDeliver.cpp
    pgr_messages.cpp

    pickDeliverEuclidean_driver.cpp
    pickDeliverEuclidean.c

    )
