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
    _pgr_pickDeliver(customers_sql, max_vehicles, capacity, [factor, max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_number, vehicle_id, stop, order_id, stop_type, cargo,
                    travel_time, arrival_time, wait_time, service_time, departure_time)



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


.. include:: VRP-category.rst
    :start-after: pd_matrix_sql_start
    :end-before: pd_matrix_sql_end


.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_start
    :end-before: pd_vehicle_sql_end


.. include:: VRP-category.rst
    :start-after: pd_parameters_start
    :end-before: pd_parameters_end

.. include:: VRP-category.rst
    :start-after: return_vrp_matrix_start
    :end-before: return_vrp_matrix_end


See Also
-------------------------------------------------------------------------------

* :ref:`VRP`
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

