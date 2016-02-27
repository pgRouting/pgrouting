.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_gsocvrppdtw:

pgr_vrppdtw - Pickup and Delivery problem 
===============================================================================

.. index:: 
	single: pgr_gsocvrppdtw(text,integer,integer)
	module: vrppdtw

Name
-------------------------------------------------------------------------------

``pgr_gsoc_vrppdtw`` — Returns optimized  solution


Synopsis
-------------------------------------------------------------------------------

Vehicle Routing Problem with Pickup and Delivery (VRPPD): A number of goods need to be moved from certain pickup locations to other delivery locations. The goal is to find optimal routes for a fleet of vehicles to visit the pickup and drop-off locations.

.. code-block:: sql

	 pgr_gsoc_vrppdtw(text sql, integer , integer;


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		select * from pgr_gsoc_vrppdtw(
                                                'select * from customer order by id'::text, 25,200
                                                                );
                                                                                                 
                                                                                                               

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:rid:   route ID
:nid:   node ID (``-1`` for the last row)
:cost:  cost to traverse to ``seq``

Examples
-------------------------------------------------------------------------------


.. code-block:: sql

	
                SELECT  * from pgr_gsoc_vrppdtw(
                                                'select * from customer order by id'::text, 25,200
                                                                );
                                                                                                 
                                                                                                 


	 seq | rid  | nid  | cost 
	-----+------+------+------
	   0 |    7 |    8 |    1
	   1 |    8 |    9 |    1
	   2 |    9 |   15 |    1
	   3 |   12 |   -1 |    0
           . |   .  |   .  |    .  
           . |   .  |   .  |    .
 



See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Vehicle_routing_problem
