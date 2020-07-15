..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_pickDeliver - Experimental
===============================================================================

``pgr_pickDeliver`` - Pickup and delivery Vehicle Routing Problem

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_pickDeliver.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_pickDeliver.html>`__

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
    single: pgr_pickDeliver - Experimental

Signature
-------------------------------------------------------------------------------

..
    TEXT, -- orders_sql
    TEXT, -- vehicles_sql
    TEXT, -- matrix_cell_sql
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

    pgr_pickDeliver(orders_sql, vehicles_sql, matrix_sql [, factor, max_cycles, initial_sol])
    RETURNS SET OF (seq, vehicle_number, vehicle_id, stop, order_id, stop_type, cargo,
                    travel_time, arrival_time, wait_time, service_time, departure_time)


Parameters
...............................................................................

The parameters are:

.. code-block:: none

    orders_sql, vehicles_sql, matrix_sql [, factor, max_cycles, initial_sol]


================= ================== ========= =================================================
Column            Type                Default    Description
================= ================== ========= =================================================
**orders_sql**    ``TEXT``                     `Pick & Deliver Orders SQL`_ query contianing the orders to be processed.
**vehicles_sql**  ``TEXT``                     `Pick & Deliver Vehicles SQL`_ query containing the vehicles to be used.
**matrix_sql**    ``TEXT``                     `Pick & Deliver Matrix SQL`_ query containing the distance or travel times.
**factor**        ``NUMERIC``          1       Travel time multiplier. See :ref:`pd_factor`
**max_cycles**    ``INTEGER``          10      Maximum number of cycles to perform on the optimization.
**initial_sol**   ``INTEGER``          4       Initial solution to be used.

                                               - ``1`` One order per truck
                                               - ``2`` Push front order.
                                               - ``3`` Push back order.
                                               - ``4`` Optimize insert.
                                               - ``5`` Push back order that allows more orders to be inserted at the back
                                               - ``6`` Push front order that allows more orders to be inserted at the front
================= ================== ========= =================================================



Pick & Deliver Orders SQL
................................................................................

A `SELECT` statement that returns the following columns:

.. code-block:: none

    id, demand
    p_node_id, p_open, p_close, [p_service, ]
    d_node_id, d_open, d_close, [d_service, ]

where:

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_general_start
    :end-before: pd_orders_sql_general_end

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_matrix_start
    :end-before: pd_orders_sql_matrix_end


Pick & Deliver Vehicles SQL
.........................................................................................

A `SELECT` statement that returns the following columns:

.. code-block:: none

    id, capacity
    start_node_id, start_open, start_close [, start_service, ]
    [ end_node_id, end_open, end_close, end_service ]

where:

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_general_start
    :end-before: pd_vehicle_sql_general_end

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_matrix_start
    :end-before: pd_vehicle_sql_matrix_end


.. end of vehicles_sql

Pick & Deliver Matrix SQL
.........................................................................................

A `SELECT` statement that returns the following columns:


.. TODO
.. warning:: TODO



.. include:: pgRouting-concepts.rst
    :start-after: where_definition_starts
    :end-before: where_definition_ends



Example
-------------------------------------------------------------------------------

.. TODO

This example use the following data: TODO put link

.. literalinclude:: doc-pickDeliver.queries
   :start-after: --q2
   :end-before: --q3


See Also
-------------------------------------------------------------------------------

* :doc:`VRP-category`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

