.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra_v3:

pgr_dijkstra - Shortest Path Dijkstra
===============================================================================

``pgr_dijkstra`` — Returns the shortest path(s) using Dijkstra algorithm.
In particular, the Dijkstra algorithm implemented by Boost.Graph.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956.
It is a graph search algorithm that solves the shortest path problem for
a graph with non-negative edge path costs, producing a shortest path from 
a starting vertex (``start_vid``) to an ending vertex (``end_vid``).
This implementation can be used with a directed graph and an undirected graph.

Signatures
===============================================================================

.. index:: 
	single: dijkstra(edges_sql, start_vid, end_vid)

.. rubric:: Minimal signature

The minimal signature is for a **directed** graph from one ``start_vid`` to one ``end_vid``:

.. code-block:: sql

      pgr_dijkstra(text edges_sql, bigint start_vid, bigint end_vid)
	 RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET




.. index:: 
	single: dijkstra(edges_sql, start_vid, end_vid, directed)

.. rubric:: Dijkstra 1 to 1


This signature performs a Dijkstra from one ``start_vid`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

.. code-block:: sql

      pgr_dijkstra(text edges_sql, bigint start_vid, bigint end_vid,
			 boolean directed:=true);
	 RETURNS SET OF (seq, path_seq, node, edge, cost, agg_cost) or EMPTY SET




.. index:: 
	single: dijkstra(edges_sql, start_vids, end_vid, directed)

.. rubric:: Dijkstra many to 1:

.. code-block:: sql

      pgr_dijkstra(text edges_sql, array[ANY_INTEGER] start_vids, bigint end_vid,
			 boolean directed:=true);
	 RETURNS SET OF (seq, path_seq, start_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to one ``end_vid``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several one to one Dijkstra
where the ending vertex is fixed.
The result is the union of the results of the one to one dijkstra.

The extra ``start_vid`` in the result is used to distinguish to which path it belongs.





.. index:: 
	single: dijkstra(edges_sql, start_vid, end_vids, directed)

.. rubric:: Dijkstra 1 to many:

.. code-block:: sql

       pgr_dijkstra(text edges_sql, bigint start_vid, array[ANY_INTEGER] end_vids,
			 boolean directed:=true);
	 RETURNS SET OF (seq, path_seq, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from one ``start_vid`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several 1 to 1 Dijkstra
where the starting vertex is fixed.
The result is the union of the results of the one to one dijkstra.

The extra ``end_vid`` in the result is used to distinguish to which path it belongs.



.. index:: 
	single: dijkstra(edges_sql, start_vids, end_vids, directed)

.. rubric:: Dijkstra many to many:

.. code-block:: sql

       pgr_dijkstra(text edges_sql, array[ANY_INTEGER] start_vids, array[ANY_INTEGER] end_vids,
			 boolean directed:=true);
	 RETURNS SET OF (seq, path_seq, start_vid, end_vid, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from each ``start_vid`` in  ``start_vids`` to each ``end_vid`` in ``end_vids``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


Using this signature, will load once the graph and perform all combinations 
for starting vertices and ending vertices.

The extra ``start_vid`` and ``end_vid`` in the result is used to distinguish to which path it belongs.


Description of the Signatures
=============================

Description of the SQL query
-------------------------------------------------------------------------------

:edges_sql: an SQL query, which should return a set of rows with the following columns:

	:id: ``ANY-INTEGER`` identifier of the edge.
	:source: ``ANY-INTEGER`` identifier of the first end point vertex of the edge.
	:target: ``ANY-INTEGER`` identifier of the second end pont vertex of the edge.
	:cost: ``ANY-NUMERICAL`` weight of the edge `(source, target)`, if negative: edge `(source, target)` does not exist, therefore it's not part of the graph.
	:reverse_cost: ``ANY-NUMERICAL`` (optional) weight of the edge `(target, source)`, if negative: edge `(target, source)` does not exist, therefore it's not part of the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float

For example:

.. code-block:: sql

    SELECT id, source, target, cost, reverse_cost from edge_table where geom && ST_Expand(ST_SetSRID(ST_Point(45, 34), 4326), 0.1)


Description of the parameters of the signatures
-------------------------------------------------------------------------------

:sql: SQL query as decribed above.
:start_vid: ``BIGINT`` identifier of the starting vertex of the path.
:start_vids: ``array[ANY-INTEGER]`` array of identifiers of starting vertices.
:end_vid: ``BIGINT`` identifier of the ending vertex of the path.
:end_vids: ``array[ANY-INTEGER]`` array of identifiers of ending vertices.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_vid] [, end_vid] , node, edge, cost, agg_cost)``

:seq: ``INT``  isequential value starting from **1**.
:path_seq: ``INT``  relative position in the path. Has value **1** for the begining of a path.
:start_vid: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:end_vid: ``BIGINT`` id of the ending vertex. Used when multiple ending vertices are in the query.
:node: ``BIGINT`` id of the node in the path from start_vid to end_v.
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path. 
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:agg_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.


Examples
========

The examples of this section are based on the :ref:`sampledata` network.

The examples include combinations from starting vertices 2 and 11 to ending vertices 3 and 5 in a directed and
undirected graph with and with out reverse_cost.

Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, 3
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |    8 |    1 |        1
       3 |        3 |    6 |    9 |    1 |        2
       4 |        4 |    9 |   16 |    1 |        3
       5 |        5 |    4 |    3 |    1 |        4
       6 |        6 |    3 |   -1 |    0 |        5
    (6 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, 5
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |   -1 |    0 |        1
    (2 rows)

When you pass an array we get a combined result:

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, array[3,5]
    );

    seq | path_seq | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+------+------+------+----------
      1 |        1 |       3 |    2 |    4 |    1 |        0
      2 |        2 |       3 |    5 |    8 |    1 |        1
      3 |        3 |       3 |    6 |    9 |    1 |        2
      4 |        4 |       3 |    9 |   16 |    1 |        3
      5 |        5 |       3 |    4 |    3 |    1 |        4
      6 |        6 |       3 |    3 |   -1 |    0 |        5
      7 |        1 |       5 |    2 |    4 |    1 |        0
      8 |        2 |       5 |    5 |   -1 |    0 |        1
   (8 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            11, 3
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |   11 |   13 |    1 |        0
       2 |        2 |   12 |   15 |    1 |        1
       3 |        3 |    9 |   16 |    1 |        2
       4 |        4 |    4 |    3 |    1 |        3
       5 |        5 |    3 |   -1 |    0 |        4
    (5 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            11, 5
            );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |   11 |   13 |    1 |        0
       2 |        2 |   12 |   15 |    1 |        1
       3 |        3 |    9 |    9 |    1 |        2
       4 |        4 |    6 |    8 |    1 |        3
       5 |        5 |    5 |   -1 |    0 |        4
    (5 rows)

Some other combinations.

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2,11], 5
    );

    seq | path_seq | start_vid | node | edge | cost | agg_cost 
   -----+----------+---------+------+------+------+----------
      1 |        1 |         2 |    2 |    4 |    1 |        0
      2 |        2 |         2 |    5 |   -1 |    0 |        1
      3 |        1 |        11 |   11 |   13 |    1 |        0
      4 |        2 |        11 |   12 |   15 |    1 |        1
      5 |        3 |        11 |    9 |    9 |    1 |        2
      6 |        4 |        11 |    6 |    8 |    1 |        3
      7 |        5 |        11 |    5 |   -1 |    0 |        4
   (7 rows)



    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2, 11], array[3,5]
    );
    seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+-------+------+------+------+----------
      1 |        1 |         2 |     3 |    2 |    4 |    1 |        0
      2 |        2 |         2 |     3 |    5 |    8 |    1 |        1
      3 |        3 |         2 |     3 |    6 |    9 |    1 |        2
      4 |        4 |         2 |     3 |    9 |   16 |    1 |        3
      5 |        5 |         2 |     3 |    4 |    3 |    1 |        4
      6 |        6 |         2 |     3 |    3 |   -1 |    0 |        5
      7 |        1 |         2 |     5 |    2 |    4 |    1 |        0
      8 |        2 |         2 |     5 |    5 |   -1 |    0 |        1
      9 |        1 |        11 |     3 |   11 |   13 |    1 |        0
     10 |        2 |        11 |     3 |   12 |   15 |    1 |        1
     11 |        3 |        11 |     3 |    9 |   16 |    1 |        2
     12 |        4 |        11 |     3 |    4 |    3 |    1 |        3
     13 |        5 |        11 |     3 |    3 |   -1 |    0 |        4
     14 |        1 |        11 |     5 |   11 |   13 |    1 |        0
     15 |        2 |        11 |     5 |   12 |   15 |    1 |        1
     16 |        3 |        11 |     5 |    9 |    9 |    1 |        2
     17 |        4 |        11 |     5 |    6 |    8 |    1 |        3
     18 |        5 |        11 |     5 |    5 |   -1 |    0 |        4
   (18 rows)




Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, 3,
            false
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    2 |    1 |        0
       2 |        2 |    3 |   -1 |    0 |        1
    (2 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, 5,
            false
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |   -1 |    0 |        1
    (2 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            11, 3,
            false
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |   11 |   11 |    1 |        0
       2 |        2 |    6 |    5 |    1 |        1
       3 |        3 |    3 |   -1 |    0 |        2
    (3 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            11, 5,
            false
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |   11 |   11 |    1 |        0
       2 |        2 |    6 |    8 |    1 |        1
       3 |        3 |    5 |   -1 |    0 |        2
    (3 rows)


       
    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2,11], 5,
            false
            );
    seq | path_seq | start_vid | node | edge | cost | agg_cost 
   -----+----------+---------+------+------+------+----------
      1 |        1 |         2 |    2 |    4 |    1 |        0
      2 |        2 |         2 |    5 |   -1 |    0 |        1
      3 |        1 |        11 |   11 |   11 |    1 |        0
      4 |        2 |        11 |    6 |    8 |    1 |        1
      5 |        3 |        11 |    5 |   -1 |    0 |        2
   (5 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, array[3,5],
            false
    );
    seq | path_seq | end_vid | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |       3 |    2 |    2 |    1 |        0
      2 |        2 |       3 |    3 |   -1 |    0 |        1
      3 |        1 |       5 |    2 |    4 |    1 |        0
      4 |        2 |       5 |    5 |   -1 |    0 |        1
   (4 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2, 11], array[3,5],
            false
    );
    seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+-------+------+------+------+----------
      1 |        1 |         2 |       3 |    2 |    2 |    1 |        0
      2 |        2 |         2 |       3 |    3 |   -1 |    0 |        1
      3 |        1 |         2 |       5 |    2 |    4 |    1 |        0
      4 |        2 |         2 |       5 |    5 |   -1 |    0 |        1
      5 |        1 |        11 |       3 |   11 |   11 |    1 |        0
      6 |        2 |        11 |       3 |    6 |    5 |    1 |        1
      7 |        3 |        11 |       3 |    3 |   -1 |    0 |        2
      8 |        1 |        11 |       5 |   11 |   11 |    1 |        0
      9 |        2 |        11 |       5 |    6 |    8 |    1 |        1
     10 |        3 |        11 |       5 |    5 |   -1 |    0 |        2
   (10 rows)

    

Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            2, 3
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
    (0 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            2, 5
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |   -1 |    0 |        1
    (2 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            11, 3
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
    (0 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            11, 5
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
    (0 rows)

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2,11], 5
    );
     seq | path_seq | start_vid | node | edge | cost | agg_cost 
    -----+----------+---------+------+------+------+----------
       1 |        1 |         2 |    2 |    4 |    1 |        0
       2 |        2 |         2 |    5 |   -1 |    0 |        1
    (2 rows)
   

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            2, array[3,5]
    );
     seq | path_seq | end_vid | node | edge | cost | agg_cost 
    -----+----------+-------+------+------+------+----------
       1 |        1 |       5 |    2 |    4 |    1 |        0
       2 |        2 |       5 |    5 |   -1 |    0 |        1
      (2 rows)
   

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost FROM edge_table',
            array[2, 11], array[3,5]
    );
     seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
    -----+----------+---------+-------+------+------+------+----------
       1 |        1 |         2 |       5 |    2 |    4 |    1 |        0
       2 |        2 |         2 |       5 |    5 |   -1 |    0 |        1
    (2 rows)

    

Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`

.. code-block:: sql

    SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost FROM edge_table',
		2, 3,
        false
    );
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |    8 |    1 |        1
       3 |        3 |    6 |    5 |    1 |        2
       4 |        4 |    3 |   -1 |    0 |        3
    (4 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 5,
                    false
	);
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |   -1 |    0 |        1
    (2 rows)


	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 3,
                    false
		);
	 seq | path_seq | node | edge | cost | agg_cost 
	-----+----------+------+------+------+----------
       1 |        1 |   11 |   11 |    1 |        0
       2 |        2 |    6 |    5 |    1 |        1
       3 |        3 |    3 |   -1 |    0 |        2
    (3 rows)


	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 5,
                    false
		);
     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |   11 |   11 |    1 |        0
       2 |        2 |    6 |    8 |    1 |        1
       3 |        3 |    5 |   -1 |    0 |        2
    (3 rows)

       
	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2,11], 5,
            false
	);
    seq | path_seq | start_vid | node | edge | cost | agg_cost 
   -----+----------+---------+------+------+------+----------
      1 |        1 |         2 |    2 |    4 |    1 |        0
      2 |        2 |         2 |    5 |   -1 |    0 |        1
      3 |        1 |        11 |   11 |   11 |    1 |        0
      4 |        2 |        11 |    6 |    8 |    1 |        1
      5 |        3 |        11 |    5 |   -1 |    0 |        2
   (5 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, array[3,5],
            false
	);
    seq | path_seq | end_vid | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |       3 |    2 |    4 |    1 |        0
      2 |        2 |       3 |    5 |    8 |    1 |        1
      3 |        3 |       3 |    6 |    5 |    1 |        2
      4 |        4 |       3 |    3 |   -1 |    0 |        3
      5 |        1 |       5 |    2 |    4 |    1 |        0
      6 |        2 |       5 |    5 |   -1 |    0 |        1
   (6 rows)


	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2, 11], array[3,5],
            false
	);
    seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+-------+------+------+------+----------
      1 |        1 |         2 |       3 |    2 |    4 |    1 |        0
      2 |        2 |         2 |       3 |    5 |    8 |    1 |        1
      3 |        3 |         2 |       3 |    6 |    5 |    1 |        2
      4 |        4 |         2 |       3 |    3 |   -1 |    0 |        3
      5 |        1 |         2 |       5 |    2 |    4 |    1 |        0
      6 |        2 |         2 |       5 |    5 |   -1 |    0 |        1
      7 |        1 |        11 |       3 |   11 |   11 |    1 |        0
      8 |        2 |        11 |       3 |    6 |    5 |    1 |        1
      9 |        3 |        11 |       3 |    3 |   -1 |    0 |        2
     10 |        1 |        11 |       5 |   11 |   11 |    1 |        0
     11 |        2 |        11 |       5 |    6 |    8 |    1 |        1
     12 |        3 |        11 |       5 |    5 |   -1 |    0 |        2
   (12 rows)




Equvalences between signatures
------------------------------

.. code-block:: sql

    -- V2
	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
        true,    -- directed flag
        true      -- has_rcost
	);

     seq | id1 | id2 | cost 
    -----+-----+-----+------
       0 |   2 |   4 |    1
       1 |   5 |   8 |    1
       2 |   6 |   9 |    1
       3 |   9 |  16 |    1
       4 |   4 |   3 |    1
       5 |   3 |  -1 |    0
      (6 rows)


    -- V3
	SELECT * FROM pgr_dijkstra(
           'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
        true     -- directed flag
	);


	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2,3 
	);

     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    4 |    1 |        0
       2 |        2 |    5 |    8 |    1 |        1
       3 |        3 |    6 |    9 |    1 |        2
       4 |        4 |    9 |   16 |    1 |        3
       5 |        5 |    4 |    3 |    1 |        4
       6 |        6 |    3 |   -1 |    0 |        5
    (6 rows)


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, array[3],
            true     
    );

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, array[3]
    );

    seq | path_seq | end_vid | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |       3 |    2 |    4 |    1 |        0
      2 |        2 |       3 |    5 |    8 |    1 |        1
      3 |        3 |       3 |    6 |    9 |    1 |        2
      4 |        4 |       3 |    9 |   16 |    1 |        3
      5 |        5 |       3 |    4 |    3 |    1 |        4
      6 |        6 |       3 |    3 |   -1 |    0 |        5
   (6 rows)

       

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], array[3],
            true
    );


    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], array[3]
    );
    seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+-------+------+------+------+----------
      1 |        1 |         2 |     3 |    2 |    4 |    1 |        0
      2 |        2 |         2 |     3 |    5 |    8 |    1 |        1
      3 |        3 |         2 |     3 |    6 |    9 |    1 |        2
      4 |        4 |         2 |     3 |    9 |   16 |    1 |        3
      5 |        5 |         2 |     3 |    4 |    3 |    1 |        4
      6 |        6 |         2 |     3 |    3 |   -1 |    0 |        5
   (6 rows)





Equivalences  between signatures 
-------------------------------------------------------------------------------

.. code-block:: sql

    -- V2
	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
            false,    -- directed flag
            true      -- has_rcost
	);

     seq | id1 | id2 | cost 
    -----+-----+-----+------
       0 |   2 |   2 |    1
       1 |   3 |  -1 |    0
    (2 rows)


    -- V3
	SELECT * FROM pgr_dijkstra(
           'SELECT id, source, target, cost, reverse_cost FROM edge_table',
		2, 3,
            false     -- directed flag
	);

     seq | path_seq | node | edge | cost | agg_cost 
    -----+----------+------+------+------+----------
       1 |        1 |    2 |    2 |    1 |        0
       2 |        2 |    3 |   -1 |    0 |        1
    (2 rows)



    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            2, array[3],
            false     
    );
    seq | path_seq | end_vid | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |       3 |    2 |    2 |    1 |        0
      2 |        2 |       3 |    3 |   -1 |    0 |        1
   (2 rows)



    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], 3,
            false
    );
    seq | path_seq | start_vid | node | edge | cost | agg_cost 
   -----+----------+---------+------+------+------+----------
      1 |        1 |         2 |    2 |    2 |    1 |        0
      2 |        2 |         2 |    3 |   -1 |    0 |        1
   (2 rows)
   

    SELECT * FROM pgr_dijkstra(
            'SELECT id, source, target, cost, reverse_cost FROM edge_table',
            array[2], array[3],
            false
    );
    seq | path_seq | start_vid | end_vid | node | edge | cost | agg_cost 
   -----+----------+---------+-------+------+------+------+----------
      1 |        1 |         2 |       3 |    2 |    2 |    1 |        0
      2 |        2 |         2 |       3 |    3 |   -1 |    0 |        1
   (2 rows)



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality in version 2.1.0


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

