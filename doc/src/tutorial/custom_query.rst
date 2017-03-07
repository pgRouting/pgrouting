.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _custom_query:

Queries & Results
===============================================================================

There are several kinds of valid inner queries and also the columns returned are debending of the function..
Which kind of **edges_sql** will depend on the function(s) requirements.

Inner Queries
-------------------------------------------------------------------------------

.. basic_edges_sql_start

Description of the edges_sql query for dijkstra like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`
                                                   - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,
                                                   - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. basic_edges_sql_end

.. no_id_edges_sql_start

Description of the edges_sql query (id is not necessary)
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`
                                                  * When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,
                                                  - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT

.. no_id_edges_sql_end


.. xy_edges_sql_start

Description of the edges_sql query for astar like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

================  ===================   ======== =================================================
Column            Type                  Default  Description
================  ===================   ======== =================================================
**id**            ``ANY-INTEGER``                Identifier of the edge.
**source**        ``ANY-INTEGER``                Identifier of the first end point vertex of the edge.
**target**        ``ANY-INTEGER``                Identifier of the second end point vertex of the edge.
**cost**          ``ANY-NUMERICAL``              Weight of the edge  `(source, target)`
                                                   - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_cost**  ``ANY-NUMERICAL``       -1     Weight of the edge `(target, source)`,
                                                   - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

**x1**            ``ANY-NUMERICAL``              X coordinate of `source` vertex.
**y1**            ``ANY-NUMERICAL``              Y coordinate of `source` vertex.
**x2**            ``ANY-NUMERICAL``              X coordinate of `target` vertex.
**y2**            ``ANY-NUMERICAL``              Y coordinate of `target` vertex.
================  ===================   ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT
:ANY-NUMERICAL: SMALLINT, INTEGER, BIGINT, REAL, FLOAT


.. xy_edges_sql_end

.. flow_edges_sql_start

Description of the edges_sql query for Max-flow like functions
...............................................................................

:edges_sql: an SQL query, which should return a set of rows with the following columns:

==================== =================== ======== =================================================
Column               Type                Default  Description
==================== =================== ======== =================================================
**id**               ``ANY-INTEGER``              Identifier of the edge.
**source**           ``ANY-INTEGER``              Identifier of the first end point vertex of the edge.
**target**           ``ANY-INTEGER``              Identifier of the second end point vertex of the edge.
**capacity**         ``ANY-INTEGER``              Weight of the edge  `(source, target)`
                                                   - When negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
**reverse_capacity** ``ANY-INTEGER``       -1     Weight of the edge `(target, source)`,
                                                   - When negative: edge `(target, source)` does not exist, therefore it's not part of the graph.
==================== =================== ======== =================================================

Where:

:ANY-INTEGER: SMALLINT, INTEGER, BIGINT


.. flow_edges_sql_end



.. points_sql_start

Description of the Points SQL query
...............................................................................

:points_sql: an SQL query, which should return a set of rows with the following columns:

============ ================= =================================================
Column            Type              Description
============ ================= =================================================
**pid**      ``ANY-INTEGER``   (optional) Identifier of the point.
                                 - If column present, it can not be NULL. 
                                 - If column not present, a sequential identifier will be given automatically.

**edge_id**  ``ANY-INTEGER``   Identifier of the "closest" edge to the point.
**fraction** ``ANY-NUMERICAL`` Value in <0,1> that indicates the relative postition from the first end point of the edge.
**side**     ``CHAR``          (optional) Value in ['b', 'r', 'l', NULL] indicating if the point is:
                                 - In the right, left of the edge or
                                 - If it doesn't matter with 'b' or NULL.
                                 - If column not present 'b' is considered.
============ ================= =================================================

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

.. points_sql_end


.. pd_euclidean_sql_start


Description of the orders SQL query for pickDeliveryEucledian
...............................................................................

=================== ===================   ================ =================================================
Column              Type                  Default          Description
=================== ===================   ================ =================================================
**id**              ``ANY-INTEGER``                        Identifier of the order.
**demand**          ``ANY-NUMERICAL``                      How much is added / removed from the vehicle. ::math::`Demand > 0`
**pick_x**          ``ANY-NUMERICAL``                      ``X`` coordinate of the pickup location.
**pick_y**          ``ANY-NUMERICAL``                      ``Y`` coordinate of the pickup location.
**deliver_x**       ``ANY-NUMERICAL``                      ``X`` coordinate of the delivery location.
**deliver_y**       ``ANY-NUMERICAL``                      ``Y`` coordinate of the delivery location.

**pick_opens**      ``ANY-NUMERICAL``     0                The time relative to 0, when the pickup location opens.
**pick_closes**     ``ANY-NUMERICAL``     :math:`\infty`   The time relative to 0, when the pickup location closes.
**pick_service**    ``ANY-NUMERICAL``     0                The duration of the pickup service.
**deliver_opens**   ``ANY-NUMERICAL``     pick_opens       The time relative to 0, when the delivery location opens.
**deliver_closes**  ``ANY-NUMERICAL``     pick_closes      The time relative to 0, when the delivery location closes.
**deliver_service** ``ANY-NUMERICAL``     pick_service     The duration of the delivery service.
=================== ===================   ================ =================================================


Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

.. pd_euclidean_sql_end

.. pd_vehicle_sql_start


Description of the Vehicles SQL query for pickDeliveryEucledian
...............................................................................

=================== ===================   =============== =================================================
Column              Type                  Default         Description
=================== ===================   =============== =================================================
**id**               ``ANY-INTEGER``                      Identifier of the order.
**capacity**         ``ANY-NUMERICAL``                    Capacity of the vehicle.
**start_x**          ``ANY-NUMERICAL``                    ``X`` coordinate of the starting location.
**start_y**          ``ANY-NUMERICAL``                    ``Y`` coordinate of the ending location.

**speed**            ``ANY-NUMERICAL``     1              Speed of the vehicle
**number**           ``ANY-NUMERICAL``     1              Number of vehicles.
**start_opens**      ``ANY-NUMERICAL``     0              The time relative to 0, when the pickup location opens.
**start_closes**     ``ANY-NUMERICAL``     :math:`\infty` The time relative to 0, when the pickup location closes.
**start_service**    ``ANY-NUMERICAL``     0              The duration of the pickup service.

**end_x**            ``ANY-NUMERICAL``     start_x        ``X`` coordinate of the delivery location.
**end_y**            ``ANY-NUMERICAL``     start_y        ``Y`` coordinate of the delivery location.
**end_opens**        ``ANY-NUMERICAL``     start_opens    The time relative to 0, when the delivery location opens.
**end_closes**       ``ANY-NUMERICAL``     start_closes   The time relative to 0, when the delivery location closes.
**end_service**      ``ANY-NUMERICAL``     start_service  The duration of the delivery service.
=================== ===================   =============== =================================================


Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

.. pd_vehicle_sql_end




Description of the return values
--------------------------------------------------------------------------------


.. return_path_start

Description of the return values for a path
...............................................................................

Returns set of ``(seq, path_seq [, start_vid] [, end_vid], node, edge, cost, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**seq**        ``INT``    Sequential value starting from **1**.
**path_seq**   ``INT``    Relative position in the path. Has value **1** for the beginning of a path.
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**node**       ``BIGINT`` Identifier of the node in the path from ``start_vid`` to ``end_vid``.
**edge**       ``BIGINT`` Identifier of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path.
**cost**       ``FLOAT``  Cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_v`` to ``node``.
============== ========== =================================================

.. return_path_end

.. return_cost_start

Description of the return values for a Cost function
...............................................................................

Returns set of ``(start_vid, end_vid, agg_cost)``

============== ========== =================================================
Column         Type       Description
============== ========== =================================================
**start_vid**  ``BIGINT`` Identifier of the starting vertex. Used when multiple starting vetrices are in the query.
**end_vid**    ``BIGINT`` Identifier of the ending vertex. Used when multiple ending vertices are in the query.
**agg_cost**   ``FLOAT``  Aggregate cost from ``start_vid`` to ``end_vid``.
============== ========== =================================================

.. return_cost_end

.. return_vrp_start

Description of the Vehicle Routing Problem functions
...............................................................................

Returns set of ``(seq, vehicle_number, vehicle_id, stop, order_id, stop_type, cargo, travel_time, arrival_time, wait_time, service_time, departure_time)``

=================== ============= =================================================
Column              Type            Description
=================== ============= =================================================
**seq**              ``INTEGER``    Sequential value starting from **1**.
**vehicle_number**   ``INTEGER``    Sequential numbering of the vehicles.

                                      * Starting from **1** for the beginning of a path.
                                      * ``-2`` is used to indicate the row is a **summary**

**vehicle_id**       ``BIGINT``     Relative position in the path. Has value **1** for the beginning of a path.

                                      * ``-1`` is used to indicate the vehicle is "made up"
                                      * Holds the number of times there is a time windows violation when the row is a **summary**

**stop**             ``INTEGER``    Stop number of the vehicle. Has value **1** for the beginning of the trip.

                                      * Holds the number of times there is a capacity violation when the row is a **summary**

**order_id**         ``BIGINT``     Order identifier that is attended on the current ``stop``.

                                      * ``-1`` when its the `starting site` or the `ending site`

**stop_type**        ``INTEGER``    Identifier of the ending vertex. Used when multiple ending vertices are in the query.

                                      * ``1`` when its the `starting` location
                                      * ``2`` when its a `pickup` location
                                      * ``3`` when its an `deliver` location
                                      * ``5`` when its the `ending` location

**cargo**            ``FLOAT``      Cargo of the vehicle before departing from the ``stop``.

**travel_time**      ``FLOAT``      Travel time from the previous ``vehicle_seq`` to the current. ``0`` for the first stop of the vehicle.

                                      * ``departure time`` of the previous ``stop`` plus the ``travel_time``
                                      * Holds the total travel_time when the row is a **summary**

**wait_time**        ``FLOAT``      Waiting time at the ``stop``.

                                      * ``0`` when the vehicle arrives within the ``stop``'s time window.
                                      * ``> 0`` when the vehicle arrives before the ``stop`` opens.
                                      * Holds the total waiting_time when the row is a **summary**

**service_time**     ``FLOAT``      Service time at the ``stop``.

                                      * Holds the total service_time when the row is a **summary**

**departure_time**   ``FLOAT``      Time at which the vehicle departs

                                      * ``departure time`` of the previous ``stop`` plus the ``travel_time`` plus the ``wait_time`` plus the ``service_time``.
                                      * Holds the total duration time when the row is a **summary**

=================== ============= =================================================



.. return_vrp_end
