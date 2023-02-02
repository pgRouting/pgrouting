..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

|

* **Supported versions:**
  `Latest <https://docs.pgrouting.org/latest/en/pgr_pickDeliver.html>`__
  (`3.5 <https://docs.pgrouting.org/3.5/en/pgr_pickDeliver.html>`__)
  `3.4 <https://docs.pgrouting.org/3.4/en/pgr_pickDeliver.html>`__
  `3.3 <https://docs.pgrouting.org/3.3/en/pgr_pickDeliver.html>`__
  `3.2 <https://docs.pgrouting.org/3.2/en/pgr_pickDeliver.html>`__
  `3.1 <https://docs.pgrouting.org/3.1/en/pgr_pickDeliver.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_pickDeliver.html>`__

``pgr_pickDeliver`` - Experimental
===============================================================================

``pgr_pickDeliver`` - Pickup and delivery Vehicle Routing Problem

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function


Synopsis
-------------------------------------------------------------------------------

Problem: Distribute and optimize the pickup-delivery pairs into a fleet of
vehicles.

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


Characteristics
-------------------------------------------------------------------------------

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

.. index::
    single: pgr_pickDeliver - Experimental on v3.0

Signature
-------------------------------------------------------------------------------

.. admonition:: \ \
   :class: signatures

   | pgr_pickDeliver(`Orders SQL`_, `Vehicles SQL`_, `Matrix SQL`_, [**options**])
   | **options:** ``[factor, max_cycles, initial_sol]``

   | RETURNS SET OF |result-pickdrop|

:Example: Solve the following problem

Given the vehicles:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: -- q1
   :end-before: -- q2

and the orders:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: -- q2
   :end-before: -- q3

The query:

.. literalinclude:: doc-pickDeliver.queries
   :start-after: -- q3
   :end-before: -- q4

Parameters
...............................................................................

The parameters are:

.. include:: VRP-category.rst
    :start-after: pd_parameters_start
    :end-before: pd_parameters_end

Pick-Deliver optional parameters
...............................................................................

.. include:: VRP-category.rst
    :start-after: pd_optionals_start
    :end-before: pd_optionals_end

Orders SQL
................................................................................

A `SELECT` statement that returns the following columns:

.. admonition:: \ \
   :class: signatures

   | id, demand
   | p_node_id, p_open, p_close, [p_service,]
   | d_node_id, d_open, d_close, [d_service,]

where:

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_general_start
    :end-before: pd_orders_sql_general_end

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_matrix_start
    :end-before: pd_orders_sql_matrix_end


Vehicles SQL
...............................................................................

A `SELECT` statement that returns the following columns:

.. admonition:: \ \
   :class: signatures

   | id, capacity
   | start_node_id, start_open, start_close [, start_service,]
   | [end_node_id, end_open, end_close, end_service]

where:

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_general_start
    :end-before: pd_vehicle_sql_general_end

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_matrix_start
    :end-before: pd_vehicle_sql_matrix_end


Matrix SQL
...............................................................................

.. include:: pgRouting-concepts.rst
    :start-after: where_definition_starts
    :end-before: where_definition_ends

Return columns
-------------------------------------------------------------------------------

.. include:: VRP-category.rst
    :start-after: pd_returns_start
    :end-before: pd_returns_end

See Also
-------------------------------------------------------------------------------

* :doc:`VRP-category`
* :doc:`sampledata`

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

