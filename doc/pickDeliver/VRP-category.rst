..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _VRP:

Vehicle Routing Functions Category:
===============================================================================


.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. index from here
.. rubric:: Pickup and delivery problem

- :ref:`pgr_pickDeliver` - Pickup & Delivery using a Cost Matrix
- :ref:`pgr_pickDeliverEuclidean` - Pickup & Delivery with Euclidean distances
- :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)

.. rubric:: Delivery prolem

-  :ref:`pgr_vrp_basic` - VRP One Depot

.. index to here

.. toctree::
  :hidden:

  pgr_pickDeliver
  pgr_pickDeliverEuclidean
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot


Introduction
-------------------------------------------------------------------------------

Vehicle routing problems (VRP) are **NP-hard** optimization problem, it generalises the travelling salesman problem (TSP).
Most of the literature use euclidean calculation.

- The objective of the VRP is to minimize the total route cost.
- There are several variants of the VRP problem,
- pgRouting does not try to implement all variants.


Characteristics
...............................................................................

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.


Pick & Delivery
-------------------------------------------------------------------------------


Problem: Distribute and optimize the pickup-delivery orders into a fleet of capacitated vehicles and
orders and vehicles are with time windows.

- Times are relative to `0` and can represent:

  - The beginning of the day
  - The beginning of the month
  - 9 am

- The vehicles

  - have start and ending service duration times.
  - have opening and closing times for the start and ending locations.

- The orders

  - Have pick up and delivery locations.
  - Have opening and closing times for the pickup and delivery locations.
  - Have pickup and delivery duration service times.

- Time based calculations:

  - Travel time between customers is :math:`distance / speed`
  - Pickup and delivery order pair is done by the same vehicle.
  - A pickup is done before the delivery.

- In case of Euclidean signatures:

  - The vehicles have :math:`(x, y)` start and ending locations.
  - The orders Have :math:`(x, y)` pickup and delivery locations.



Units handling
-------------------------------------------------------------------------------

.. TODO:: Explain how to handle units between vehicle capacity, order demand, cargo results


Time handling
-------------------------------------------------------------------------------

.. TODO:: Explain how time is handled internaly and some steps the user might need to make it work




Inner Queries
-------------------------------------------------------------------------------


.. rubric:: Pick & Delivery

Here is a summary of the Inner queries used in 

- :ref:`pgr_pickDeliver`
- :ref:`pgr_pickDeliverEuclidean`

.. pd_matrix_sql_start


.. warning:: TODO: FIX becuase the following is outdated

Description of the orders_sql query
.........................................................................................

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the customer.

                                        - A value of ``0`` identifies the starting location

**demand**        ``ANY-NUMERICAL``     How much is added / removed from the vehicle.

                                        - Negative value is a delivery,
                                        - Positive value is a pickup,

**openTime**      ``ANY-NUMERICAL``     The time relative to 0, when the customer opens.
**closeTime**     ``ANY-NUMERICAL``     The time relative to 0, when the customer closes.
**serviceTime**   ``ANY-NUMERICAL``     The duration of the loading / unloading.
**pickup_id**     ``ANY-INTEGER``       Value used when the current customer is a Delivery to find the corresponding Pickup
**deliver_id**    ``ANY-INTEGER``       Value used when the current customer is a Pickup to find the corresponding Delivery
================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. pd_matrix_sql_end

.. pd_euclidean_sql_start

.. warning:: TODO: FIX becuase the following is outdated

Description of the orders_sql query
.........................................................................................

================  ===================   =================================================
Column            Type                  Description
================  ===================   =================================================
**id**            ``ANY-INTEGER``       Identifier of the customer.

                                        - A value of ``0`` identifies the starting location

**x**             ``ANY-NUMERICAL``     ``X`` coordinate of the location.
**y**             ``ANY-NUMERICAL``     ``Y`` coordinate of the location.
**demand**        ``ANY-NUMERICAL``     How much is added / removed from the vehicle.

                                        - Negative value is a delivery,
                                        - Positive value is a pickup,

**openTime**      ``ANY-NUMERICAL``     The time relative to 0, when the customer opens.
**closeTime**     ``ANY-NUMERICAL``     The time relative to 0, when the customer closes.
**serviceTime**   ``ANY-NUMERICAL``     The duration of the loading / unloading.
**pickup_id**     ``ANY-INTEGER``       Value used when the current customer is a Delivery to find the corresponding Pickup
**deliver_id**    ``ANY-INTEGER``       Value used when the current customer is a Pickup to find the corresponding Delivery
================  ===================   =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. pd_euclidean_sql_end

.. pd_vehicle_sql_start

.. warning:: TODO: Write


Description of the vehicles_sql query
.........................................................................................

.. pd_vehicle_sql_end

.. pd_parameters_start

.. warning:: TODO: FIX becuase the following is outdated

Description of the parameters of the signatures
...............................................................................

================= ================== ========= =================================================
Column            Type                Default    Description
================= ================== ========= =================================================
**orders_sql**    ``TEXT``                     Orders SQL query as described above.
**vehicles_sql**  ``TEXT``                     Vehciles SQL query as described above.
**factor**        ``NUMERIC``          1       $multiplier for the travel time
**max_cycles**    ``INTEGER``          10      Array of identifiers of starting vertices.
**initial**       ``INTEGER``          4       Identifier of the ending vertex of the path.

                                                  * ``1`` One order per truck
                                                  * ``2`` push front
                                                  * ``3`` push back
                                                  * ``4`` best insert
                                                  * ``5`` order that allows more orders to be inserted at the back
                                                  * ``6`` order that allows more orders to be inserted at the front

================= ================== ========= =================================================

.. pd_parameters_end


Results
-------------------------------------------------------------------------------

..
    OUT seq INTEGER,
    OUT vehicle_seq INTEGER,
    OUT vehicle_id BIGINT,
    OUT stop_seq INTEGER,
    OUT order_id BIGINT,
    OUT stop_type INT,
    OUT cargo FLOAT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT

.. return_vrp_euclidean_start

Description of the result
.........................................................................................

.. TODO: fix when everything bellow is fixed

:RETURNS SET OF: (seq, vehicle_id, vehicle_seq, stop_id, travel_time, arrival_time, wait_time, service_time,  departure_time)
           UNION (summary row)

=================== ============= =================================================
Column              Type           Description
=================== ============= =================================================
**seq**              INTEGER      Sequential value starting from **1**.
**vehicle_seq**      INTEGER      Sequential value starting from **1** for current vehicles. The :math:`n_{th}` vehicle in the solution.
**vehicle_id**       BIGINT       Current vehicle identifier.
**stop_seq**         INTEGER      Sequential value starting frin **1** for the stops made by the current vechile. The :math:`m_{th}` stop of the current vehicle.
**stop_type**        INTEGER      Kind of stop location the vehicle is at:

                                  - ``1``: Starting location 
                                  - ``2``: Pickup location 
                                  - ``3``: Delivery location 
                                  - ``6``: Ending location 

**order_id**         BIGINT       Pickup-Delivery order pair identifier.

                                  - ``-1``: When no order is involved on the current stop location. 

**cargo**            FLOAT        Cargo units of the vehicle when leaving the stop.

**travel_time**      FLOAT        Travel time from previous ``vehicle_seq`` to current ``vehicle_seq``.

                                  - ``0`` When ``stop_type = 1``

**arrival_time**     FLOAT        Previous ``departure_time`` plus current ``travel_time``.
**wait_time**        FLOAT        Time spent waiting for current `location` to open.
**service_time**     FLOAT        Service time at current `location`.
**departure_time**   FLOAT        :math:`arrival\_time + wait\_time + service\_time`.

                                  - When ``stop_type = 6`` has the `total_time` used for the current vehicle.
=================== ============= =================================================

.. rubric:: Summary Row

=================== ============= =================================================
Column              Type           Description
=================== ============= =================================================
**seq**              INTEGER      Continues the Sequential value
**vehicle_seq**      INTEGER      ``-2`` to indicate is a summary row
**vehicle_id**       BIGINT       `Total Capacity Violations` in the solution.
**stop_seq**         INTEGER      `Total Time Window Violations` in the solution.
**stop_type**        INTEGER      ``-1``
**order_id**         BIGINT       ``-1``
**cargo**            FLOAT        ``-1``
**travel_time**      FLOAT        `total_travel_time` The sum of all the `travel_time`
**arrival_time**     FLOAT        ``-1``
**wait_time**        FLOAT        `total_waiting_time` The sum of all the `wait_time`
**service_time**     FLOAT        `total_service_time` The sum of all the `service_time`
**departure_time**   FLOAT        `total_solution_time` = :math:`total\_travel\_time + total\_wait\_time + total\_service\_time`.
=================== ============= =================================================



.. return_vrp_euclidean_end



.. return_vrp_matrix_start

.. include:: VRP-category.rst
    :start-after: return_vrp_euclidean_start
    :end-before: return_vrp_euclidean_end

.. return_vrp_matrix_end




See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Vehicle_routing_problem
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

