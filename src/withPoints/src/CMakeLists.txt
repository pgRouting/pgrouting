ADD_LIBRARY(withPoints OBJECT 
                            one_to_one_withPoints.c 
                            one_to_many_withPoints.c 
                            many_to_one_withPoints.c 
                            many_to_many_withPoints.c 


                            get_new_queries.cpp
                            pgr_withPoints.cpp

                            one_to_one_withPoints_driver.cpp
                            many_to_one_withPoints_driver.cpp
                            many_to_many_withPoints_driver.cpp
                            one_to_many_withPoints_driver.cpp)
