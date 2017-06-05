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


.. note:: TODO

.. rubric:: The following is a sketch

Problem: Distribute and optimize the pickup-delivery pairs into a fleet of vehicles.

- Optimization problem is NP-hard.
- Pickup and Delivery:

  - capacitated
  - with time windows.

- The vehicles

  - have (x, y) start and ending locations.
  - have a start and ending service times.
  - have opening and closing times for the start and ending locations.

- An order is for doing a pickup and a a deliver.

  - has (x, y) pickup and delivery locations.
  - has opening and closing times for the pickup and delivery locations.
  - has a pickup and deliver service times.

- There is a customer where to deliver a pickup.

  - travel time between customers is distance / speed
  - pickup and delivery pair is done with the same vehicle.
  - A pickup is done before the delivery.



Characteristics:
----------------

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- Six different optional different initial solutions



Inner Queries
-------------

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
---------

.. return_vrp_euclidean_start

.. warning:: TODO: FIX becuase the following is outdated

Description of the result
.........................................................................................

:RETURNS SET OF: (seq, vehicle_id, vehicle_seq, stop_id, travel_time, arrival_time, wait_time, service_time,  departure_time)

=================== ============= =================================================
Column              Type           Description
=================== ============= =================================================
**seq**              INTEGER      Sequential value starting from **1**.
**vehicle_id**       INTEGER      Current vehicle identifier.
**vehicle_seq**      INTEGER      Sequential value starting from **1** for the current vehicle.
**stop_id**          BIGINT       Visited customer identifier.
**travel_time**      FLOAT        Travel time from previous ``stop_id`` to current ``stop_id``.
**arrival_time**     FLOAT        Previous ``departure_time`` plus current ``travel_time``.
**wait_time**        FLOAT        Time spent waiting for ``stop_id`` to open.
**service_time**     FLOAT        Service time at current stop_id.
**departure_time**   FLOAT        Previous :math:`departure\_time + travel\_time + wait\_time + service\_time`.
                                    - When ``stop_id = 0`` and ``vehicle_seq != 1`` has the total time for the current ``vehicle_id``.
                                    - When ``vehicle_id = -1`` has the aggregate total time
=================== ============= =================================================


.. return_vrp_euclidean_end

.. return_vrp_matrix_start

.. warning:: TODO: FIX becuase the following is outdated

Description of the result
.........................................................................................

:RETURNS SET OF: (seq, vehicle_id, vehicle_seq, stop_id, travel_time, arrival_time, wait_time, service_time,  departure_time)

=================== ============= =================================================
Column              Type           Description
=================== ============= =================================================
**seq**              INTEGER      Sequential value starting from **1**.
**vehicle_id**       INTEGER      Current vehicle identifier.
**vehicle_seq**      INTEGER      Sequential value starting from **1** for the current vehicle.
**stop_id**          BIGINT       Visited customer identifier.
**travel_time**      FLOAT        Travel time from previous ``stop_id`` to current ``stop_id``.
**arrival_time**     FLOAT        Previous ``departure_time`` plus current ``travel_time``.
**wait_time**        FLOAT        Time spent waiting for ``stop_id`` to open.
**service_time**     FLOAT        Service time at current stop_id.
**departure_time**   FLOAT        Previous :math:`departure\_time + travel\_time + wait\_time + service\_time`.
                                    - When ``stop_id = 0`` and ``vehicle_seq != 1`` has the total time for the current ``vehicle_id``.
                                    - When ``vehicle_id = -1`` has the aggregate total time
=================== ============= =================================================

.. return_vrp_matrix_end




See Also
-------------------------------------------------------------------------------

* https://en.wikipedia.org/wiki/Vehicle_routing_problem
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

