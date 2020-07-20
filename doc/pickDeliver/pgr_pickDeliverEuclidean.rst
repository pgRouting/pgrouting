..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: https://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

pgr_pickDeliverEuclidean - Experimental
===============================================================================

``pgr_pickDeliverEuclidean`` - Pickup and delivery Vehicle Routing Problem

.. include:: experimental.rst
   :start-after: begin-warn-expr
   :end-before: end-warn-expr

.. rubric:: Availability

* Version 3.0.0

  * Replaces ``pgr_gsoc_vrppdtw``
  * New **experimental** function

.. rubric:: Support

* **Supported versions:**
  current(`3.1 <https://docs.pgrouting.org/3.1/en/pgr_pickDeliverEuclidean.html>`__)
  `3.0 <https://docs.pgrouting.org/3.0/en/pgr_pickDeliverEuclidean.html>`__

* **Unsupported versions:**
  `2.6 <https://docs.pgrouting.org/2.6/en/pgr_gsoc_vrppdtw.html>`__
  `2.5 <https://docs.pgrouting.org/2.5/en/pgr_gsoc_vrppdtw.html>`__
  `2.4 <https://docs.pgrouting.org/2.4/en/pgr_gsoc_vrppdtw.html>`__
  `2.3 <https://docs.pgrouting.org/2.3/en/src/vrppdtw/doc/pgr_gsoc_vrppdtw.html>`__
  `2.2 <https://docs.pgrouting.org/2.2/en/src/vrppdtw/doc/index.html>`__
  `2.1 <https://docs.pgrouting.org/2.1/en/src/vrppdtw/doc/index.html>`__


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



Characteristics
-------------------------------------------------------------------------------

- No multiple time windows for a location.
- Less vehicle used is considered better.
- Less total duration is better.
- Less wait time is better.
- Six different optional different initial solutions

  - the best solution found will be result


.. index::
    single: pgr_pickDeliverEuclidean - Experimental


Signature
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


Parameters
...............................................................................

The parameters are:

.. code-block:: none

    orders_sql, vehicles_sql [,factor, max_cycles, initial_sol]

Where:

.. include:: VRP-category.rst
    :start-after: pd_parameters_start
    :end-before: pd_parameters_end


Pick & Deliver Orders SQL
...............................................................................

A `SELECT` statement that returns the following columns:

.. code-block:: none

    id, demand
    p_x, p_y, p_open, p_close, [p_service, ]
    d_x, d_y, d_open, d_close, [d_service, ]

Where:

.. include:: VRP-category.rst
    :start-after: pd_orders_sql_general_start
    :end-before: pd_orders_sql_general_end


.. include:: VRP-category.rst
    :start-after: pd_orders_euclidean_sql_start
    :end-before: pd_orders_euclidean_sql_end



Pick & Deliver Vehicles SQL
.........................................................................................

A `SELECT` statement that returns the following columns:

.. code-block:: none

    id, capacity
    start_x, start_y, start_open, start_close [, start_service, ]
    [ end_x, end_y, end_open, end_close, end_service ]

where:

.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_general_start
    :end-before: pd_vehicle_sql_general_end


.. include:: VRP-category.rst
    :start-after: pd_vehicle_sql_euclidean_start
    :end-before: pd_vehicle_sql_euclidean_end


.. include:: VRP-category.rst
    :start-after: return_vrp_euclidean_start:
    :end-before: return_vrp_euclidean_end


.. include:: pgRouting-concepts.rst
    :start-after: where_definition_starts
    :end-before: where_definition_ends


Example
-------------------------------------------------------------------------------

.. TODO

This example use the following data: TODO put link


.. literalinclude:: ./doc-pickDeliverEuclidean.queries
   :start-after: --q1
   :end-before: --q2

See Also
-------------------------------------------------------------------------------

* :doc:`VRP-category`
* The queries use the :doc:`sampledata` network.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

