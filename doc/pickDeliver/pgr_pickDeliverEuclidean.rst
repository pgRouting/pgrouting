..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|


``pgr_pickDeliverEuclidean`` - Experimental
===============================================================================

``pgr_pickDeliverEuclidean`` - Pickup and delivery Vehicle Routing Problem

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * Replaces ``pgr_gsoc_vrppdtw``
  * New **experimental** function


Synopsis
-------------------------------------------------------------------------------

Problem: Distribute and optimize the pickup-delivery pairs into a fleet of
vehicles.

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



Characteristics
-------------------------------------------------------------------------------

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- Six different optional different initial solutions

  - the best solution found will be result


.. index::
    single: pgr_pickDeliverEuclidean - Experimental on 3.0


Signature
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_pickDeliverEuclidean(`Orders SQL`_, `Vehicles SQL`_, [**options**])
   | **options:** ``[factor, max_cycles, initial_sol]``

   | RETURNS SET OF |result-pickdrop|

:Example: Solve the following problem

Given the vehicles:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: -- q1
   :end-before: -- q2

and the orders:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: -- q2
   :end-before: -- q3

The query:

.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: -- q3
   :end-before: -- q4


Parameters
...............................................................................

.. include:: VRP-category.rst
    :start-after: pde_parameters_start
    :end-before: pde_parameters_end

Pick-Deliver optional parameters
...............................................................................

.. include:: VRP-category.rst
    :start-after: pd_optionals_start
    :end-before: pd_optionals_end

Orders SQL
...............................................................................

A `SELECT` statement that returns the following columns:

.. admonition:: \ \
   :class: signatures

   | id, demand
   | p_x, p_y, p_open, p_close, [p_service,]
   | d_x, d_y, d_open, d_close, [d_service]

Where:

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_general_start
    :end-before: pd_orders_sql_general_end


.. include:: VRP-category.rst
    :start-after: pd_orders_euclidean_sql_start
    :end-before: pd_orders_euclidean_sql_end

Vehicles SQL
...............................................................................

A `SELECT` statement that returns the following columns:

.. admonition:: \ \
   :class: signatures

   | id, capacity
   | start_x, start_y, start_open, start_close [, start_service, ]
   | [ end_x, end_y, end_open, end_close, end_service ]

where:

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_general_start
    :end-before: pd_vehicle_sql_general_end


.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_euclidean_start
    :end-before: pd_vehicle_sql_euclidean_end


Return columns
-------------------------------------------------------------------------------

.. include:: VRP-category.rst
    :start-after: pd_returns_start
    :end-before: pd_returns_end

Example
-------------------------------------------------------------------------------
.. contents::
   :local:

.. include:: sampledata.rst
    :start-after: pd_data_start
    :end-before: pd_data_end

The query
...............................................................................

Showing only the relevant information to compare with the best solution
information published on
https://www.sintef.no/projectweb/top/pdptw/100-customers/

* The best solution found for **lc101** is a travel time: 828.94
* This implementation's travel time: 854.54

.. literalinclude:: ./lc101.queries
   :start-after: -- q4
   :end-before: -- q5

See Also
-------------------------------------------------------------------------------

* :doc:`VRP-category`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

