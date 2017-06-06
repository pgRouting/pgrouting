..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_pickDeliverEuclidean:

__pgr_pickDeliverEuclidean:
===============================================================================

.. warning:: Under development: Changes in code/documentation are without notice

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
----------------

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- Six different optional different initial solutions

  - the best solution found will be result


Signature Summary
-----------------

.. code-block:: none

    _pgr_pickDeliverEuclidean(orders_sql, vehicles_sql, [,factor, max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_number, vehicle_id, stop, order_id, stop_type, cargo,
                    travel_time, arrival_time, wait_time, service_time, departure_time)




Signatures
-------------------------------------------------------------------------------


.. index::
    single: _pgr_pickDeliverEuclidean(Complete Signature)

pgr_pickDeliverEuclidean(Complete Signature)
................................................

.. code-block:: none

    _pgr_pickDeliverEuclidean(orders_sql, vehicles_sql [,factor , max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_number, vehicle_id, stop, order_id, stop_type, cargo,
                    travel_time, arrival_time, wait_time, service_time, departure_time)

:Example:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: --q1
   :end-before: --q2


Description of the Signatures
-------------------------------------------------------------------------------

.. include:: VRP-category.rst
    :start-after: pd_euclidean_sql_start
    :end-before: pd_euclidean_sql_end


.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_start
    :end-before: pd_vehicle_sql_end


.. include:: VRP-category.rst
    :start-after: pd_parameters_start
    :end-before: pd_parameters_end

.. include:: VRP-category.rst
    :start-after: return_vrp_euclidean_start
    :end-before: return_vrp_euclidean_end



See Also
-------------------------------------------------------------------------------

* :ref:`VRP`
* The queries use the :ref:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

