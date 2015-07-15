.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_apsp_warshall:

pgr_apspWarshall - All Pairs Shortest Path, Floyd-Warshall Algorithm
===============================================================================

.. index::
    single: pgr_apspWarshall(text, boolean, boolean)
    module: apsp

Name
-------------------------------------------------------------------------------

``pgr_apspWarshall`` - Returns all costs for each pair of nodes in the graph.


Synopsis
-------------------------------------------------------------------------------

The Floyd-Warshall algorithm (also known as Floyd's algorithm and other names) is a graph analysis algorithm for finding the shortest paths between all pairs of nodes in a weighted graph. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows for every pair of nodes in the graph.

.. code-block:: sql

    pgr_costResult[] pgr_apspWarshall(sql text, directed boolean, reverse_cost boolean);


Description
-------------------------------------------------------------------------------

:sql: a SQL query that should return the edges for the graph that will be analyzed:

    .. code-block:: sql

        SELECT id, source, target, cost FROM edge_table;

    :id: ``int4`` identifier of the edge
    :source: ``int4`` identifier of the source vertex for this edge
    :target: ``int4`` identifier of the target vertex for this edge
    :cost: ``float8`` a positive value for the cost to traverse this edge

:directed: ``true`` if the graph is directed
:reverse_cost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

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
        FROM pgr_apspWarshall(
            'SELECT id, source, target, cost FROM edge_table',
            false, false
        );

     seq | from | to | cost 
    -----+------+----+------
       0 |    1 |  1 |    0
       1 |    1 |  2 |    1
       2 |    1 |  3 |    0
       3 |    1 |  4 |   -1
    [...]

The query uses the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* :ref:`pgr_apsp_johnson`
* http://en.wikipedia.org/wiki/Floyd%E2%80%93Warshall_algorithm
