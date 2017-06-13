..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_pickDeliverEuclidean:

pgr_pickDeliverEuclidean:
===============================================================================

.. warning:: Documentation is being updated

.. include:: proposed.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr


Synopsis
-------------------------------------------------------------------------------

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
-------------------------------------------------------------------------------

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- Six different optional different initial solutions

  - the best solution found will be result


Signature Summary
-------------------------------------------------------------------------------

..  
    TEXT, -- orders_sql
    TEXT, -- vehicles_sql
    factor FLOAT DEFAULT 1,
    max_cycles INTEGER DEFAULT 10,
    initial_sol INTEGER DEFAULT 4,


..
    OUT seq INTEGER,
    OUT vehicle_seq INTEGER,
    OUT vehicle_id BIGINT,
    OUT stop_seq INTEGER,
    OUT stop_type INTEGER,
    OUT order_id BIGINT,
    OUT cargo FLOAT,
    OUT travel_time FLOAT,
    OUT arrival_time FLOAT,
    OUT wait_time FLOAT,
    OUT service_time FLOAT,
    OUT departure_time FLOAT


.. code-block:: none

    pgr_pickDeliverEuclidean(orders_sql, vehicles_sql [,factor, max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_seq, vehicle_id, stop_seq, stop_type, order_id,
        cargo, travel_time, arrival_time, wait_time, service_time, departure_time)




Signatures
-------------------------------------------------------------------------------


.. index::
    single: pgr_pickDeliverEuclidean(Complete Signature)

pgr_pickDeliverEuclidean(Complete Signature)
-------------------------------................................................

.. code-block:: none

    pgr_pickDeliverEuclidean(orders_sql, vehicles_sql [,factor, max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_seq, vehicle_id, stop_seq, stop_type, order_id,
        cargo, travel_time, arrival_time, wait_time, service_time, departure_time)

:Example:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: --q1
   :end-before: --q2


Description of the Signatures
-------------------------------------------------------------------------------

.. include:: VRP-category.rst
    :start-after: pd_euclidean_sql_start:
    :end-before: pd_euclidean_sql_end


.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_start:
    :end-before: pd_vehicle_sql_end


.. include:: VRP-category.rst
    :start-after: pd_parameters_start
    :end-before: pd_parameters_end

.. include:: VRP-category.rst
    :start-after: return_vrp_euclidean_start:
    :end-before: return_vrp_euclidean_end



See Also
-------------------------------------------------------------------------------

* :ref:`VRP`
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

