.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_apsp_johnson:

pgr_apspJohnson - All Pairs Shortest Path, Johnson's Algorithm
===============================================================================

.. index::
    single: pgr_apspJohnson(text)
    module: apsp

Name
-------------------------------------------------------------------------------

``pgr_apspJohnson`` - Returns all costs for each pair of nodes in the graph.


Synopsis
-------------------------------------------------------------------------------

Johnson's algorithm is a way to find the shortest paths between all pairs of vertices in a sparse, edge weighted, directed graph. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows for every pair of nodes in the graph.

.. code-block:: sql

    pgr_costResult[] pgr_apspJohnson(sql text);


Description
-------------------------------------------------------------------------------

:sql: a SQL query that should return the edges for the graph that will be analyzed:

    .. code-block:: sql

        SELECT source, target, cost FROM edge_table;

    :source: ``int4`` identifier of the source vertex for this edge
    :target: ``int4`` identifier of the target vertex for this edge
    :cost: ``float8`` a positive value for the cost to traverse this edge

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   source node ID
:id2:   target node ID
:cost:  cost to traverse from ``id1`` to ``id2``

.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

    SELECT seq, id1 AS from, id2 AS to, cost 
        FROM pgr_apspJohnson(
            'SELECT source, target, cost FROM edge_table'
        );

     seq | from | to | cost 
    -----+------+----+------
       0 |    1 |  1 |    0
       1 |    1 |  2 |    1
       2 |    1 |  5 |    2
       3 |    1 |  6 |    3
    [...]

The query uses the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* :ref:`pgr_apsp_warshall`
* http://en.wikipedia.org/wiki/Johnson%27s_algorithm
