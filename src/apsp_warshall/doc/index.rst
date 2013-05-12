..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _apsp-warshall-index:

All Pairs Shortest Path - Floyd-Warshall Algorithm
==================================================

.. index::
    single: PGR_apspWarshall(text)
    module: apsp

Name
-------------------------------------------------------------------------------

``PGR_apspWarshall`` - Returns all costs for each pair of nodes in the graph.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_apspEdge <type_pgr_apspEdge>` (source_id, target_id, cost) rows for every pair of nodes in the graph.

.. code-block:: sql

    pgr_apspEdge[] PGR_apspWarshall(sql text);


Description
-------------------------------------------------------------------------------

:sql: a SQL query that should return the edges for the graph that will be analyzed:

    .. code-block:: sql

        SELECT source, target, cost FROM edge_table;

    :source: ``int4`` identifier of the source vertex for this edge
    :target: ``int4`` identifier of the target vertex for this edge
    :cost: ``float8`` a positive value for the cost to traverse this edge


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT source_id, target_id, cost 
        FROM pgr_apspWarshall(
            'SELECT source, target, cost FROM edge_table'
        );


