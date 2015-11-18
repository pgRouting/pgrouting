.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstraViaVertices:

pgr_dijkstraViaVertices - Shortest paths via vertices using dijkstra 
===============================================================================

.. index:: 
	single: pgr_pgr_dijkstraViaVertices(text,integer,integer,boolean,boolean)
	module: dijkstra

Name
-------------------------------------------------------------------------------

``pgr_dijkstraViaVertices`` — Using dijkstra algorithm, it finds the route that goes through
a list of vertices.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Given a list of vertices and a graph, this function is equivalent to finding the
shortest path between $vertex_i$ and $vertex_{i+1}$


Signatures
===============================================================================

.. rubric:: pgr_dijkstraViaVertices Minimal signature

The minimal signature is for a **directed** graph from one ``start_v`` to one ``end_v``:

.. code-block:: sql

      pgr_dijkstraViaVertices(sql_q text, array[ANY_INTEGER] via_vertex)
       	 RETURNS SET OF (seq, start_v, end_v, node, edge, cost, agg_cost) or EMPTY SET


.. rubric:: pgr_dijkstraViaVertices complete signature

This signature works: 
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


.. code-block:: sql

      pgr_dijkstraViaVertices(sql_q text, array[ANY_INTEGER] via_vertex)
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

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11]
                );

        SELECT * FROM pgr_dijkstraViaVertices(
                  'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                  array[1, 3, 11],
                  true
                );

 	 seq | start_v | end_v | vertex | edge | cost | agg_cost 
	 ----+---------+-------+--------+------+------+----------
   	   1 |       1 |     3 |      1 |    1 |    1 |        0
   	   2 |       1 |     3 |      2 |    4 |    1 |        1
   	   3 |       1 |     3 |      5 |    8 |    1 |        2
   	   4 |       1 |     3 |      6 |    9 |    1 |        3
   	   5 |       1 |     3 |      9 |   16 |    1 |        4
   	   6 |       1 |     3 |      4 |    3 |    1 |        5
   	   7 |       1 |     3 |      3 |   -1 |    0 |        6
   	   8 |       3 |    11 |      3 |    5 |    1 |        6
   	   9 |       3 |    11 |      6 |   11 |    1 |        7
          10 |       3 |    11 |     11 |   -1 |    0 |        8
	(10 rows)




Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[1, 3, 11],
			 false
                );

 	seq | start_v | end_v | vertex | edge | cost | agg_cost 
	----+---------+-------+--------+------+------+----------
   	  1 |       1 |     3 |      1 |    1 |    1 |        0
   	  2 |       1 |     3 |      2 |    2 |    1 |        1
   	  3 |       1 |     3 |      3 |   -1 |    0 |        2
   	  4 |       3 |    11 |      3 |    5 |    1 |        2
   	  5 |       3 |    11 |      6 |   11 |    1 |        3
   	  6 |       3 |    11 |     11 |   -1 |    0 |        4
	(6 rows)


Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

In this example there is no route because there is no path from 1 to 3.

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11]
                );

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11],
			 true
                );

 	 seq | start_v | end_v | vertex | edge | cost | agg_cost 
	----+---------+-------+--------+------+------+----------
	(0 rows)



Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. code-block:: sql

        SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11],
                        false
		);

	 seq | start_v | end_v | vertex | edge | cost | agg_cost 
	-----+---------+-------+--------+------+------+----------
	   1 |       1 |     3 |      1 |    1 |    1 |        0
	   2 |       1 |     3 |      2 |    4 |    1 |        1
	   3 |       1 |     3 |      5 |    8 |    1 |        2
	   4 |       1 |     3 |      6 |    5 |    1 |        3
	   5 |       1 |     3 |      3 |   -1 |    0 |        4
	   6 |       3 |    11 |      3 |    5 |    1 |        4
	   7 |       3 |    11 |      6 |   11 |    1 |        5
	   8 |       3 |    11 |     11 |   -1 |    0 |        6
	(8 rows)



Equvalences between pgr_dijkstraViaVertices and pgr_dijkstra
------------------------------------------------------------

This example shows how the equivalence between pgr_dijkstraViaVertices and
pgr_dijkstra can be obtained.

We want a round trip going from vertces: 1 -> 3 -> 11 -> 1

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[1], array[3]
        );
     seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
    -----+----------+-----------+---------+------+------+------+----------
       1 |        1 |         1 |       3 |    1 |    1 |    1 |        0
       2 |        2 |         1 |       3 |    2 |    4 |    1 |        1
       3 |        3 |         1 |       3 |    5 |    8 |    1 |        2
       4 |        4 |         1 |       3 |    6 |    9 |    1 |        3
       5 |        5 |         1 |       3 |    9 |   16 |    1 |        4
       6 |        6 |         1 |       3 |    4 |    3 |    1 |        5
       7 |        7 |         1 |       3 |    3 |   -1 |    0 |        6
    (7 rows)

    SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[3], array[11]
        );
     seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
    -----+----------+-----------+---------+------+------+------+----------
       1 |        1 |         3 |      11 |    3 |    5 |    1 |        0
       2 |        2 |         3 |      11 |    6 |   11 |    1 |        1
       3 |        3 |         3 |      11 |   11 |   -1 |    0 |        2
    (3 rows)

    SELECT * FROM pgr_dijkstra(
               'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[11], array[1]
        );
     seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
    -----+----------+-----------+---------+------+------+------+----------
       1 |        1 |        11 |       1 |   11 |   13 |    1 |        0
       2 |        2 |        11 |       1 |   12 |   15 |    1 |        1
       3 |        3 |        11 |       1 |    9 |   16 |    1 |        2
       4 |        4 |        11 |       1 |    4 |    3 |    1 |        3
       5 |        5 |        11 |       1 |    3 |    2 |    1 |        4
       6 |        6 |        11 |       1 |    2 |    1 |    1 |        5
       7 |        7 |        11 |       1 |    1 |   -1 |    0 |        6
    (7 rows)

    SELECT * FROM pgr_dijkstraViaVertices(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[1, 3, 11, 1],
                        false
        );
     seq | start_v | end_v | vertex | edge | cost | agg_cost 
    -----+---------+-------+--------+------+------+----------
       1 |       1 |     3 |      1 |    1 |    1 |        0
       2 |       1 |     3 |      2 |    4 |    1 |        1
       3 |       1 |     3 |      5 |    8 |    1 |        2
       4 |       1 |     3 |      6 |    5 |    1 |        3
       5 |       1 |     3 |      3 |   -1 |    0 |        4
       6 |       3 |    11 |      3 |    5 |    1 |        4
       7 |       3 |    11 |      6 |   11 |    1 |        5
       8 |       3 |    11 |     11 |   -1 |    0 |        6
       9 |      11 |     1 |     11 |   11 |    1 |        6
      10 |      11 |     1 |      6 |    8 |    1 |        7
      11 |      11 |     1 |      5 |    4 |    1 |        8
      12 |      11 |     1 |      2 |    1 |    1 |        9
      13 |      11 |     1 |      1 |   -1 |    0 |       10
    (13 rows)


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
