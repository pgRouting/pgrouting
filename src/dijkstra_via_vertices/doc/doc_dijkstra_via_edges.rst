.. 
   ****************************************************************************
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraViaEdges

pgr_dijkstraViaEdges - Shortest paths via points in edges using dijkstra 
===============================================================================

.. index:: 
	single: pgr_pgr_dijkstraViaEdges(text, array::integer, array::float, boolean)
	module: dijkstra

Name
-------------------------------------------------------------------------------

``pgr_dijkstraViaEdges`` — Using dijkstra algorithm, it finds the route that goes through
a list of points in edges.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Given a graph and a list of positional vertices given as list of edges and the corresponding
relative position of the vertex given as a fraction, this function finds the route that goes
Thru all the positional vertices.
this function is equivalent to finding the shortest path between $p_vertex_i$ and $p_vertex_{i+1}$


Signatures
===============================================================================

.. rubric:: pgr_dijkstraViaEdges Minimal signature

The minimal signature is for a **directed** graph.

.. code-block:: sql

      pgr_dijkstraViaEdges(sql_q text, via_edge array[ANY_INTEGER], via_pos arra[FLOAT8])
       	 RETURNS SET OF (seq, start_v, end_v, node, edge, cost, agg_cost) or EMPTY SET


.. rubric:: pgr_dijkstraViaEdges complete signature

This signature works: 
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. code-block:: sql

      pgr_dijkstraViaEdges(sql_q text, via_edge array[ANY_INTEGER], via_pos arra[FLOAT8])
                 boolean directed:=true);
       	 RETURNS SET OF (seq, start_v, end_v, node, edge, cost, agg_cost) or EMPTY SET


Description of the Signatures
=============================

Description of the SQL query
-------------------------------------------------------------------------------

:sql: an SQL query, which should return a set of rows with the following columns:

	:id: ``ANY-INTEGER`` identifier of the edge.
	:source: ``ANY-INTEGER`` identifier of the source vertex of the edge.
	:target: ``ANY-INTEGER`` identifier of the target vertex of the edge.
	:cost: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
	:reverse_cost: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

For example:

.. code-block:: sql

    SELECT id, source, target, cost, reverse_cost from edge_table where geom && ST_Expand(ST_SetSRID(ST_Point(45, 34), 4326))


Description of the parameters of the signatures
-------------------------------------------------------------------------------

:sql: SQL query as decribed above.
:via_vertex: ``array[ANY-INTEGER]`` array of id of the vertices in the via.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq , start_v, end_v , node, edge, cost, agg_cost)``

:seq: ``INT``  relative position in the route. Has value **1** for the begining of the route.
:start_v: ``BIGINT`` id of the via vertex from where the section starts.
:end_v: ``BIGINT`` id of the via vertex from where the section ends.
:node: ``BIGINT`` id of the node in the path from start_v to end_v.
       When ``node = start_v`` a section of the route starts. (starting via point)
       When ``node = end_v`` a section of the route ends (via point has being reached).
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the section. ``-1`` a via point has being reached. 
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the iroute.
:agg_cost:  ``FLOAT`` total cost from the first node in the via_vertex to ``node``.


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaEdges(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85]
                );

        SELECT * FROM pgr_dijkstraViaEdges(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85]
                  true
        );

     seq | start_v | end_v | vertex | edge | cost | agg_cost 
    -----+---------+-------+--------+------+------+----------
       1 |      -1 |    -2 |     -1 |    1 |  0.9 |        0
       2 |      -1 |    -2 |      2 |    4 |    1 |      0.9
       3 |      -1 |    -2 |      5 |    8 |    1 |      1.9
       4 |      -1 |    -2 |      6 |    9 |    1 |      2.9
       5 |      -1 |    -2 |      9 |   16 |    1 |      3.9
       6 |      -1 |    -2 |      4 |    3 |  0.5 |      4.9
       7 |      -1 |    -2 |     -2 |   -1 |    0 |      5.4
       8 |      -2 |    -3 |     -2 |    3 |  0.5 |      5.4
       9 |      -2 |    -3 |      3 |    5 |    1 |      5.9
      10 |      -2 |    -3 |      6 |   11 | 0.85 |      6.9
      11 |      -2 |    -3 |     -3 |   -1 |    0 |     7.75
    (11 rows)



Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaEdges(
                    'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
		    false
               );

     seq | start_v | end_v | vertex | edge | cost | agg_cost 
    -----+---------+-------+--------+------+------+----------
       1 |      -1 |    -2 |     -1 |    1 |  0.1 |        0
       2 |      -1 |    -2 |      2 |    2 |    1 |      0.1
       3 |      -1 |    -2 |      3 |    3 |  0.5 |      1.1
       4 |      -1 |    -2 |     -2 |   -1 |    0 |      1.6
       5 |      -2 |    -3 |     -2 |    3 |  0.5 |      1.6
       6 |      -2 |    -3 |      3 |    5 |    1 |      2.1
       7 |      -2 |    -3 |      6 |   11 | 0.85 |      3.1
       8 |      -2 |    -3 |     -3 |   -1 |    0 |     3.95
    (8 rows)


Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

In this example there is no route because there is no path from 1 to 3.

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85]
                );

        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
			 true
                );

 	 seq | start_v | end_v | vertex | edge | cost | agg_cost 
	----+---------+-------+--------+------+------+----------
	(0 rows)



Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaEdges(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11], array[0.1, 0.5, 0.85],
                        false
		);

	 seq | start_v | end_v | vertex | edge | cost | agg_cost 
	-----+---------+-------+--------+------+------+----------
	(0 rows)



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
