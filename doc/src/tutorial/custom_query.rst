.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _custom_query:

Inner Queries
===============================================================================

There are several kinds of valid inner queries.
Which kind of **edges_sql** will depend on the function(s) requirements.


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


