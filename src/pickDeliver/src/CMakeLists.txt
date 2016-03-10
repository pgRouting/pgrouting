ADD_LIBRARY(pickDeliver OBJECT 
    pickDeliver.c 
    customers_input.c  
    pdp_solver.cpp 
    pickDeliver_driver.cpp
    
    basictypes.cpp
    
    point.cpp
    node.cpp
    tw_node.cpp
    vehicle_node.cpp

    vehicle.cpp
    order.cpp
    )
