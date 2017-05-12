..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

   INSTRUCTIONS
   - if section consists of only one value then use this file as index.rst
   - change [...] (including the square braquets) to appropriate values
   - one file / function,  may signatures of the same function go in the same file

.. _pgr_pickDeliver:

__pgr_pickDeliver
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_pickDeliver`` - Pickup and delivery Vehicle Routing Problem

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr



Synopsis
-------------------------------------------------------------------------------

Problem: Distribute and optimize the pickup-delivery pairs into a fleet of vehicles.

- Optimization problem is NP-hard.
- pickup and Delivery with time windows.
- All vehicles are equal.

  - Same Starting location.
  - Same Ending location which is the same as Starting location.
  - All vehicles travel at the same speed.

- A customer is for doing a pickup or doing a deliver.

  - has an open time.
  - has a closing time.
  - has a service time.
  - has an (x, y) location.

- There is a customer where to deliver a pickup.

  - travel time between customers is distance / speed
  - pickup and delivery pair is done with the same vehicle.
  - A pickup is done before the delivery.


Characteristics:
----------------

- All trucks depart at time 0.
- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- the algorithm will raise an exception when

  - If there is a pickup-deliver pair than violates time window
  - The speed, max_cycles, ma_capacity have illegal values

- Six different initial will be optimized
  - the best solution found will be result


Signature Summary
-----------------

.. code-block:: none

    _pgr_pickDeliver(customers_sql, max_vehicles, capacity)
    _pgr_pickDeliver(customers_sql, max_vehicles, capacity, speed, max_cycles)
    RETURNS SET OF (seq, vehicle_id, vehicle_seq, stop_id,
         travel_time, arrival_time, wait_time, service_time,  departure_time)



Signatures
-----------

..
    Minimal signature
    .. index::
    single: _pgr_pickDeliver(Minimal Signature) - developing

Minimal signature
...................

..
   Small description, example:

The minimal signature is for `speed = 1`, for a `max_cycles = 30`

.. code-block:: none

    _pgr_pickDeliver(customers_sql, max_vehicles, capacity)
    RETURNS SET OF (seq, vehicle_id, vehicle_seq, stop_id,
         travel_time, arrival_time, wait_time, service_time,  departure_time)

:Example:

This example use the following data: TODO put link

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q1
   :end-before: --q2

..
    Complete signature
    .. index::
    single: _pgr_pickDeliver(Cmplete Signature) - developing


Complete signature
....................

This signature performs the optimization based on the optional parameters


.. code-block:: none

    _pgr_pickDeliver(customers_sql, max_vehicles, capacity, speed, max_cycles)
    RETURNS SET OF (seq, vehicle_id, vehicle_seq, stop_id,
         travel_time, arrival_time, wait_time, service_time,  departure_time)


:Example:

This example use the following data: TODO put link

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q2
   :end-before: --q3

Description of the Signatures
-------------------------------


Description of the customers_sql query
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


Description of the parameters of the signatures
.........................................................................................

================== =========== ======== =================================================
Column             Type        Default     Description
================== =========== ======== =================================================
**customers_sql**  ``TEXT``             SQL query as described above.
**max_vehicles**   ``INTEGER``          Maximum number of vehicles in the result. (currently is ignored)
**capacity**       ``FLOAT``            Capacity of the vehicle.
**speed**          ``FLOAT``   1        Speed of the vehicle.
**max_cycles**     ``INTEGER`` 30       A multiplier for internal cycles (currently is ignored)
================== =========== ======== =================================================

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



See Also
-------------------------------------------------------------------------------

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

