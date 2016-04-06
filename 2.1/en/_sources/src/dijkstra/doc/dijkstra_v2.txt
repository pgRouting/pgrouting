.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra_v2:

pgr_dijkstra (V 2.0)- Shortest Path Dijkstra
===============================================================================

Name
-------------------------------------------------------------------------------

``pgr_dijkstra`` — Returns the shortest path using Dijkstra algorithm.


.. index:: 
	single: dijkstra(sql, source, target, directed, has_rcost) -- deprecated

Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956. It is a graph search algorithm that solves the single-source shortest path problem for a graph with non-negative edge path costs, producing a shortest path tree. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_dijkstra(text sql, integer source, integer target, 
	                           boolean directed, boolean has_rcost);

.. warning:: This signature is being deprecated in version 2.1, Please use it
             without the ``has_rcost`` flag instead:

             ``pgr_dijkstra(sql, source, target, directed)``

             See :ref:`pgr_dijkstra_v3`

Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:reverse_cost: ``float8`` (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:target: ``int4`` id of the end point
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``


.. rubric:: History

* Renamed in version 2.0.0


Examples: Directed
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. code-block:: sql

        SELECT seq, id1 AS node, id2 AS edge, cost 
                FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2,3, true, false
                );

         seq | node | edge | cost 
        -----+------+------+------
        (0 rows)


* With ``reverse_cost``

.. code-block:: sql

        SELECT seq, id1 AS node, id2 AS edge, cost 
                FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2,3, true, true
                );
         seq | node | edge | cost 
        -----+------+------+------
           0 |    2 |    4 |    1
           1 |    5 |    8 |    1
           2 |    6 |    9 |    1
           3 |    9 |   16 |    1
           4 |    4 |    3 |    1
           5 |    3 |   -1 |    0
        (6 rows)





Examples: Undirected
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. code-block:: sql

       SELECT seq, id1 AS node, id2 AS edge, cost 
                FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3, false, false
                );
        seq | node | edge | cost 
       -----+------+------+------
          0 |    2 |    4 |    1
          1 |    5 |    8 |    1
          2 |    6 |    5 |    1
          3 |    3 |   -1 |    0
       (4 rows)


* With ``reverse_cost``

.. code-block:: sql

       SELECT seq, id1 AS node, id2 AS edge, cost 
                FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3, false, true
                );
        seq | node | edge | cost 
       -----+------+------+------
          0 |    2 |    2 |    1
          1 |    3 |   -1 |    0
       (2 rows)


The queries use the :ref:`sampledata` network.

See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
