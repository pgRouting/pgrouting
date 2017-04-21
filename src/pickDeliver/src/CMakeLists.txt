ADD_LIBRARY(pickDeliver OBJECT
    book_keeping.cpp
    node.cpp
    tw_node.cpp
    vehicle_node.cpp
    order.cpp
    orders.cpp
    fleet.cpp
    vehicle.cpp
    pd_problem.cpp
    vehicle_pickDeliver.cpp

    solution.cpp
    initial_solution.cpp
    optimize.cpp

    pgr_pickDeliver.cpp
    pgr_messages.cpp

    pickDeliverEuclidean_driver.cpp
    pickDeliverEuclidean.c

    pickDeliver_driver.cpp
    pickDeliver.c

    )
