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

.. rubric:: Experimental functions

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


- :ref:`pgr_gsocvrppdtw` - VRP Pickup & Delivery (Euclidean)
- :ref:`pgr_vrp_basic` - VRP One Depot

.. index to here

.. toctree::
  :hidden:

  pgr_pickDeliver
  pgr_pickDeliverEuclidean
  pgr_gsoc_vrppdtw
  pgr_vrpOneDepot


Introduction
-------------------------------------------------------------------------------

Vehicle Routing Problems `VRP` are **NP-hard** optimization problem, it generalises the travelling salesman problem (TSP).

- The objective of the VRP is to minimize the total route cost.
- There are several variants of the VRP problem,

**pgRouting does not try to implement all variants.**


Characteristics
...............................................................................

- Capacitated Vehicle Routing Problem `CVRP` where The vehicles have limited carrying capacity of the goods.
- Vehicle Routing Problem with Time Windows `VRPTW` where the locations have time windows within which the vheicles visits must be made.
- Vehicle Routing Problem with Pickup and Delivery `VRPPD` where a number of goods need to be moved from certain pickup locations to other delivery locations.


.. Rubric:: Limitations

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.


Pick & Delivery
-------------------------------------------------------------------------------

Problem: `CVRPPDTW` Capacitated Pick and Delivery Vehichle Routing problem with Time Windows

- Times are relative to `0`
- The vehicles

  - have start and ending service duration times.
  - have opening and closing times for the start and ending locations.
  - have a capacity.

- The orders

  - Have pick up and delivery locations.
  - Have opening and closing times for the pickup and delivery locations.
  - Have pickup and delivery duration service times.
  - have a demand resquest for moving goods from the pickup location to the delivery location.

- Time based calculations:

  - Travel time between customers is :math:`distance / speed`
  - Pickup and delivery order pair is done by the same vehicle.
  - A pickup is done before the delivery.

Locations
...............................................................................

- When using the Euclidean signatures:

  - The vehicles have :math:`(x, y)` pairs for start and ending locations.
  - The orders Have :math:`(x, y)` pairs for pickup and delivery locations.

- When using a matrix:

  - The vehicles have identifiers for the start and ending locations.
  - The orders have identifiers for the pickup and delivery locations.
  - All the identifiers are indices to the given matix.



Time handling
-------------------------------------------------------------------------------

**The times are relative to 0**

Suppose that a vehicle's driver starts the shift at 9:00 am and ends the shift at 4:30 pm
and the service time duration is 10 minutes with 30 seconds.

All time units have to be converted

============ ================= ==================== ===================== =========
Meaning of 0   time units       9:00 am              4:30 pm               10 mins 30 secs
============ ================= ==================== ===================== =========
0:00 am         hours            9                  16.5                   :math:`10.5 / 60  = 0.175`
9:00 am         hours            0                  7.5                    :math:`10.5 / 60  = 0.175`
0:00 am         minutes          :math:`9*60 = 54`  :math:`16.5*60 = 990`  10.5 
9:00 am         minutes          0                  :math:`7.5*60 = 540`   10.5
============ ================= ==================== ===================== =========


Capacity and Demand Units Handling
-------------------------------------------------------------------------------

The capacity of the truck, can be measured in

- volumne units like m^3
- Number of boxes that fit in it
- Number weight unit like kg
- Surface of the floor when no stacking is allowed
- Number of people that can transport, etc

The demand resquest of the pickyp-delivery orders must use the same units as the units used in the trucks capacity.

To handle problems like:  10 boxes of apples and 5 kg of feathers that are to be transported.

- boxes that would be used for `1` kg of fethers:  `f_boxes`
- weight of a `1` box of apple:  `a_weight`

=============== ====================== ==================
Capacity Units  apples                  feathers
=============== ====================== ==================
boxes            10                     :math:`5 * f\_boxes`
kg              :math:`10 * a\_weight`       5
=============== ====================== ==================









Inner Queries
-------------------------------------------------------------------------------

.. rubric:: orders_sql

- :ref:`Description of the orders_sql query <pd_matrix_sql_start>`
- :ref:`Description of the orders_sql query for Euclidean version <pd_euclidean_sql_start>`

.. rubric:: vehicles_sql

- :ref:`Description of the vehicles_sql query <pd_vehicle_sql_start>`
- :ref:`Description of the vehicles_sql query for Euclidean version <pd_vehicle_sql_euclidean_start>`

.. rubric:: return columns

- :ref:`Description of return columns <return_vrp_matrix_start>`
- :ref:`Description of the return columns for Euclidean version <return_vrp_euclidean_start>`


..
    info[0].name = strdup("id");
    info[1].name = strdup("demand");
    info[2].name = strdup("p_x");
    info[3].name = strdup("p_y");
    info[4].name = strdup("p_open");
    info[5].name = strdup("p_close");
    info[6].name = strdup("p_service");
    info[7].name = strdup("d_x");
    info[8].name = strdup("d_y");
    info[9].name = strdup("d_open");
    info[10].name = strdup("d_close");
    info[11].name = strdup("d_service");
    info[12].name = strdup("p_node_id");
    info[13].name = strdup("d_node_id")

.. _pd_matrix_sql_start:

Description of the orders_sql query
.........................................................................................

================  ===================   =========== ================================================
Column            Type                  Default     Description
================  ===================   =========== ================================================
**id**            ``ANY-INTEGER``                   Identifier of the pick-delivery order pair.
**demand**        ``ANY-NUMERICAL``                 Number of units in the order
**p_node_id**     ``ANY-INTEGER``                   The node identifier of the pickup location, must match a node in the matrix table.
**p_open**        ``ANY-NUMERICAL``                 The time, relative to 0, when the pickup location opens.
**p_close**       ``ANY-NUMERICAL``                 The time, relative to 0, when the pickup location closes.
**d_service**     ``ANY-NUMERICAL``     0           The duration of the loading at the pickup location.
**d_node_id**     ``ANY-INTEGER``                   Node identifier of the pickup location, must match a node in the matrix table.
**d_open**        ``ANY-NUMERICAL``                 The time, relative to 0, when the delivery location opens.
**d_close**       ``ANY-NUMERICAL``                 The time, relative to 0, when the delivery location closes.
**d_service**     ``ANY-NUMERICAL``     0           The duration of the loading at the delivery location.
================  ===================   =========== ================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. pd_matrix_sql_end



.. _pd_euclidean_sql_start:


Description of the orders_sql query (Euclidean)
.........................................................................................

================  ===================   =========== ================================================
Column            Type                  Default     Description
================  ===================   =========== ================================================
**id**            ``ANY-INTEGER``                   Identifier of the pick-delivery order pair.
**demand**        ``ANY-NUMERICAL``                 Number of units in the order
**p_x**           ``ANY-NUMERICAL``                 :math:`x` value of the pick up location
**p_y**           ``ANY-NUMERICAL``                 :math:`y` value of the pick up location
**p_open**        ``ANY-NUMERICAL``                 The time, relative to 0, when the pickup location opens.
**p_close**       ``ANY-NUMERICAL``                 The time, relative to 0, when the pickup location closes.
**d_service**     ``ANY-NUMERICAL``     0           The duration of the loading at the pickup location.
**d_x**           ``ANY-NUMERICAL``                 :math:`x` value of the delivery location
**d_y**           ``ANY-NUMERICAL``                 :math:`y` value of the delivery location
**d_open**        ``ANY-NUMERICAL``                 The time, relative to 0, when the delivery location opens.
**d_close**       ``ANY-NUMERICAL``                 The time, relative to 0, when the delivery location closes.
**d_service**     ``ANY-NUMERICAL``     0           The duration of the loading at the delivery location.
================  ===================   =========== ================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. pd_euclidean_sql_end


.. _pd_vehicle_sql_start:

Description of the vehicles_sql query
.........................................................................................

.. warning:: TODO: Write

.. pd_vehicle_sql_end

.. _pd_vehicle_sql_euclidean_start:



Description of the vehicles_sql query (Euclidean)
.........................................................................................

.. warning:: TODO: Write

.. pd_vehicle_sql_euclidean_end


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

                                               - ``1`` One order per truck
                                               - ``2`` push front
                                               - ``3`` push back
                                               - ``4`` best insert
                                               - ``5`` order that allows more orders to be inserted at the back
                                               - ``6`` order that allows more orders to be inserted at the front
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

.. _return_vrp_matrix_start:

Description of the result (TODO Disussion: Euclidean & Matrix)
.........................................................................................

.. todo:: fix when everything bellow is fixed


.. code-block:: none

    RETURNS SET OF
        (seq, vehicle_seq, vehicle_id, stop_seq, stop_type,
            travel_time, arrival_time, wait_time, service_time,  departure_time)
        UNION
        (summary row)

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

**travel_time**      FLOAT        Travel time from previous ``stop_seq`` to current ``stop_seq``.

                                  - ``0`` When ``stop_type = 1``

**arrival_time**     FLOAT        Previous ``departure_time`` plus current ``travel_time``.
**wait_time**        FLOAT        Time spent waiting for current `location` to open.
**service_time**     FLOAT        Service time at current `location`.
**departure_time**   FLOAT        :math:`arrival\_time + wait\_time + service\_time`.

                                  - When ``stop_type = 6`` has the `total_time` used for the current vehicle.
=================== ============= =================================================

.. rubric:: Summary Row

.. warning:: TODO: Review the summary

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


.. return_vrp_matrix_end




.. _return_vrp_euclidean_start:

.. include:: VRP-category.rst
    :start-after: return_vrp_matrix_start:
    :end-before: return_vrp_matrix_end

.. return_vrp_euclidean_end


.. glossary::

   CVRP
      Capacitated Vehicle Routing Problem:  The vehicles have limited carrying capacity of the goods that must be delivered.

   seq
      Sequential value starting from **1**.

   order
      Order to be served on a VRP problem

   VRP
      Vehicle Routing Problem


See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Vehicle_routing_problem
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

