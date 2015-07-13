.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra_v3:

pgr_dijkstra (V 3.0) - Shortest Path Dijkstra
===============================================================================

.. index:: 
	single: pgr_dijkstra(text,integer,integer,boolean,boolean)
	module: dijkstra

Name
-------------------------------------------------------------------------------

``pgr_dijkstra`` — Returns the shortest path(s) using Dijkstra algorithm.
In particular, the Dijkstra algorithm implemented by Boost.Graph.

.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/doc/libs/1_58_0/libs/graph/doc/index.html

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956.
It is a graph search algorithm that solves the shortest path problem for
a graph with non-negative edge path costs, producing a shortest path from 
a starting vertex (``start_v``) to an ending vertex (``end_v``).

.. rubric:: Minimal signature

.. code-block:: sql

      pgr_dijkstra(text sql, bigint start_v, bigint end_v)
       	 RETURNS SET OF (seq, node, edge, cost, agg_cost) or EMPTY SET

The weighted directed graph, ``G(V,E)``, is definied by:
  - the set of vertices 
    ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``
  - the set of edges
     + when ``reverse_cost`` column is used:
    ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, reverse_cost >=0)}``
     + when ``reverse_cost`` column is *not* used:
    ``E`` = ``{ (source, target, cost) where cost >=0 }``

This is done transparently using directed Boost.Graph.

For the weighted graph defined by:
  - ``G(V,E)``

and the starting and ending vertices:
  - ``start_v`` and ``end_v``

The algorithm returns a path, if it exists, in terms of a sequence of vertices and of edges,
which is the shortest path using Dijsktra algorithm between ``start_v`` and ``end_v``, in a
set form where ``seq`` indicates the position in the path of the ``node`` / ``edge``.

Aditional information like the cost (``cost``) of the edge to be used and the
aggregate cost (``agg_cost``) from the ``start_v`` up to the ``node`` is included.

If there is no path, the resulting set is empty.

The minimal signature is for a directed graph from one ``start_v`` to one ``end_v``:


.. rubric:: Dijkstra 1 to 1


.. code-block:: sql

      pgr_dijkstra(text sql, bigint start_v, bigint end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, node, edge, cost, agg_cost) or EMPTY SET

This signature performs a Dijkstra from one ``start_v`` to one ``end_v``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

When the graph is directed it acts like the minimal signature.

The weighted undirected graph, ``G(V,E)``, is definied by:
  - the set of vertices
    ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``

  - the set of edges
     + when ``reverse_cost`` column is used:
    ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost >=0)}``
      union ``{ (target, source, reverse_cost) where cost >=0 }``  union ``{ (source, target,  reverse_cost >=0)}``

     + when ``reverse_cost`` column is *not* used:
    ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost >=0)}``

This is done transparently using undirected Boost.Graph.


.. rubric:: Dijkstra many to 1:

.. code-block:: sql

      pgr_dijkstra(text sql, array[ANY_INTEGER] start_v, bigint end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, start_v, node, edge, cost, agg_cost) or EMPTY SET

.. rubric:: Dijkstra 1 to many:

.. code-block:: sql

	SET OF (seq, end_v, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, bigint start_v, array[ANY_INTEGER] end_v,
	                 boolean directed:=true);

.. rubric:: Dijkstra many to many:

.. code-block:: sql

	SET OF (seq, start_v, end_v, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, array[ANY_INTEGER] start_v, array[ANY_INTEGER] end_v,
	                 boolean directed:=true);


Description of the SQL query
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost [,reverse_cost] FROM edge_table


	:id: ``ANY-INTEGER`` identifier of the edge.
	:source: ``ANY-INTEGER`` identifier of the source vertex of the edge.
	:target: ``ANY-INTEGER`` identifier of the target vertex of the edge.
	:cost: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
	:reverse_cost: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

Where:

:ANY-INTEGER: smallint, int, bigint
:ANY-NUMERICAL: smallint, int, bigint, real, float


Description of the parameters of the signatures
-------------------------------------------------------------------------------

:sql: SQL query as decribed above.
:start_v: ``BIGINT`` id of the starting vertex.
:start_v: ``array[ANY-INTEGER]`` array of id of starting vertices.
:end_v: ``BIGINT`` id of the ending vertex.
:end_v: ``array[ANY-INTEGER]`` array of id of ending vertices.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_v] [, end_v] , node, edge, cost, tot_cost)``

:seq: ``INT``  row sequence
:start_v: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:end_v: ``BIGINT`` id of the ending vertex. Used when multiple ending vertices are in the query.
:node: ``BIGINT`` id of the node in the path from start_v to end_v.
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the path. 
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:tot_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.



Examples for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |    2 |    4 |    1 |        0
           1 |    5 |    8 |    1 |        1
           2 |    6 |    9 |    1 |        2
           3 |    9 |   16 |    1 |        3
           4 |    4 |    3 |    1 |        4
           5 |    3 |   -1 |    0 |        5
        (6 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |    2 |    4 |    1 |        0
           1 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5]
                );
         seq | end_v | node | edge | cost | tot_cost 
        -----+-------+------+------+------+----------
           0 |     3 |    2 |    4 |    1 |        0
           1 |     3 |    5 |    8 |    1 |        1
           2 |     3 |    6 |    9 |    1 |        2
           3 |     3 |    9 |   16 |    1 |        3
           4 |     3 |    4 |    3 |    1 |        4
           5 |     3 |    3 |   -1 |    0 |        5
           6 |     5 |    2 |    4 |    1 |        0
           7 |     5 |    5 |   -1 |    0 |        1
        (8 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   11 |   13 |    1 |        0
           1 |   12 |   15 |    1 |        1
           2 |    9 |   16 |    1 |        2
           3 |    4 |    3 |    1 |        3
           4 |    3 |   -1 |    0 |        4
        (5 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   11 |   13 |    1 |        0
           1 |   12 |   15 |    1 |        1
           2 |    9 |    9 |    1 |        2
           3 |    6 |    8 |    1 |        3
           4 |    5 |   -1 |    0 |        4
        (5 rows)

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5
                );
         seq | start_v | node | edge | cost | tot_cost 
        -----+---------+------+------+------+----------
           0 |       2 |    2 |    4 |    1 |        0
           1 |       2 |    5 |   -1 |    0 |        1
           2 |      11 |   11 |   13 |    1 |        0
           3 |      11 |   12 |   15 |    1 |        1
           4 |      11 |    9 |    9 |    1 |        2
           5 |      11 |    6 |    8 |    1 |        3
           6 |      11 |    5 |   -1 |    0 |        4
        (7 rows)


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5]
                );
         seq | start_v | end_v | node | edge | cost | tot_cost 
        -----+---------+-------+------+------+------+----------
           0 |       2 |     3 |    2 |    4 |    1 |        0
           1 |       2 |     3 |    5 |    8 |    1 |        1
           2 |       2 |     3 |    6 |    9 |    1 |        2
           3 |       2 |     3 |    9 |   16 |    1 |        3
           4 |       2 |     3 |    4 |    3 |    1 |        4
           5 |       2 |     3 |    3 |   -1 |    0 |        5
           6 |       2 |     5 |    2 |    4 |    1 |        0
           7 |       2 |     5 |    5 |   -1 |    0 |        1
           8 |      11 |     3 |   11 |   13 |    1 |        0
           9 |      11 |     3 |   12 |   15 |    1 |        1
          10 |      11 |     3 |    9 |   16 |    1 |        2
          11 |      11 |     3 |    4 |    3 |    1 |        3
          12 |      11 |     3 |    3 |   -1 |    0 |        4
          13 |      11 |     5 |   11 |   13 |    1 |        0
          14 |      11 |     5 |   12 |   15 |    1 |        1
          15 |      11 |     5 |    9 |    9 |    1 |        2
          16 |      11 |     5 |    6 |    8 |    1 |        3
          17 |      11 |     5 |    5 |   -1 |    0 |        4
        (18 rows)


Examples for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3,
                        false
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |    2 |    2 |    1 |        0
           1 |    3 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5,
                        false
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |    2 |    4 |    1 |        0
           1 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3,
                        false
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   11 |   11 |    1 |        0
           1 |    6 |    5 |    1 |        1
           2 |    3 |   -1 |    0 |        2
        (3 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5,
                        false
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   11 |   11 |    1 |        0
           1 |    6 |    8 |    1 |        1
           2 |    5 |   -1 |    0 |        2
        (3 rows)

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5,
                        false
                );
         seq | start_v | node | edge | cost | tot_cost 
        -----+---------+------+------+------+----------
           0 |       2 |    2 |    4 |    1 |        0
           1 |       2 |    5 |   -1 |    0 |        1
           2 |      11 |   11 |   11 |    1 |        0
           3 |      11 |    6 |    8 |    1 |        1
           4 |      11 |    5 |   -1 |    0 |        2
        (5 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5],
                        false
                );
         seq | end_v | node | edge | cost | tot_cost 
        -----+-------+------+------+------+----------
           0 |     3 |    2 |    2 |    1 |        0
           1 |     3 |    3 |   -1 |    0 |        1
           2 |     5 |    2 |    4 |    1 |        0
           3 |     5 |    5 |   -1 |    0 |        1
        (4 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5],
                        false
                );
         seq | start_v | end_v | node | edge | cost | tot_cost 
        -----+---------+-------+------+------+------+----------
           0 |       2 |     3 |    2 |    2 |    1 |        0
           1 |       2 |     3 |    3 |   -1 |    0 |        1
           2 |       2 |     5 |    2 |    4 |    1 |        0
           3 |       2 |     5 |    5 |   -1 |    0 |        1
           4 |      11 |     3 |   11 |   11 |    1 |        0
           5 |      11 |     3 |    6 |    5 |    1 |        1
           6 |      11 |     3 |    3 |   -1 |    0 |        2
           7 |      11 |     5 |   11 |   11 |    1 |        0
           8 |      11 |     5 |    6 |    8 |    1 |        1
           9 |      11 |     5 |    5 |   -1 |    0 |        2
        

Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   -1 |   -1 |    0 |        0
        (1 row)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 5
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |    2 |    4 |    1 |        0
           1 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 3
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   -1 |   -1 |    0 |        0
        (1 row)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 5
                );
         seq | node | edge | cost | tot_cost 
        -----+------+------+------+----------
           0 |   -1 |   -1 |    0 |        0
        (1 row)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2,11], 5
                );
         seq | start_v | node | edge | cost | tot_cost 
        -----+---------+------+------+------+----------
           0 |       2 |    2 |    4 |    1 |        0
           1 |       2 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, array[3,5]
                );
         seq | end_v | node | edge | cost | tot_cost 
        -----+-------+------+------+------+----------
           0 |     5 |    2 |    4 |    1 |        0
           1 |     5 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2, 11], array[3,5]
                );
         seq | start_v | end_v | node | edge | cost | tot_cost 
        -----+---------+-------+------+------+------+----------
           0 |       2 |     5 |    2 |    4 |    1 |        0
           1 |       2 |     5 |    5 |   -1 |    0 |        1
        (2 rows)
        



Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false
		);
        seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |    2 |    4 |    1 |        0
          1 |    5 |    8 |    1 |        1
          2 |    6 |    5 |    1 |        2
          3 |    3 |   -1 |    0 |        3
       (4 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 5,
                        false
		);
        seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |    2 |    4 |    1 |        0
          1 |    5 |   -1 |    0 |        1
       (2 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 3,
                        false
		);
        seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |   11 |   11 |    1 |        0
          1 |    6 |    5 |    1 |        1
          2 |    3 |   -1 |    0 |        2
       (3 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 5,
                        false
		);
        seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |   11 |   11 |    1 |        0
          1 |    6 |    8 |    1 |        1
          2 |    5 |   -1 |    0 |        2
       (3 rows)

       
	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2,11], 5,
                        false
		);
        seq | start_v | node | edge | cost | tot_cost 
       -----+---------+------+------+------+----------
          0 |       2 |    2 |    4 |    1 |        0
          1 |       2 |    5 |   -1 |    0 |        1
          2 |      11 |   11 |   11 |    1 |        0
          3 |      11 |    6 |    8 |    1 |        1
          4 |      11 |    5 |   -1 |    0 |        2
       (5 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, array[3,5],
                        false
		);
        seq | end_v | node | edge | cost | tot_cost 
       -----+-------+------+------+------+----------
          0 |     3 |    2 |    4 |    1 |        0
          1 |     3 |    5 |    8 |    1 |        1
          2 |     3 |    6 |    5 |    1 |        2
          3 |     3 |    3 |   -1 |    0 |        3
          4 |     5 |    2 |    4 |    1 |        0
          5 |     5 |    5 |   -1 |    0 |        1
       (6 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2, 11], array[3,5],
                        false
		);
        seq | start_v | end_v | node | edge | cost | tot_cost 
       -----+---------+-------+------+------+------+----------
          0 |       2 |     3 |    2 |    4 |    1 |        0
          1 |       2 |     3 |    5 |    8 |    1 |        1
          2 |       2 |     3 |    6 |    5 |    1 |        2
          3 |       2 |     3 |    3 |   -1 |    0 |        3
          4 |       2 |     5 |    2 |    4 |    1 |        0
          5 |       2 |     5 |    5 |   -1 |    0 |        1
          6 |      11 |     3 |   11 |   11 |    1 |        0
          7 |      11 |     3 |    6 |    5 |    1 |        1
          8 |      11 |     3 |    3 |   -1 |    0 |        2
          9 |      11 |     5 |   11 |   11 |    1 |        0
         10 |      11 |     5 |    6 |    8 |    1 |        1
         11 |      11 |     5 |    5 |   -1 |    0 |        2
       (12 rows)




Equivalences for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------

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

       seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |    2 |    4 |    1 |        0
          1 |    5 |    8 |    1 |        1
          2 |    6 |    9 |    1 |        2
          3 |    9 |   16 |    1 |        3
          4 |    4 |    3 |    1 |        4
          5 |    3 |   -1 |    0 |        5
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

       seq | start_v | node | edge | cost | tot_cost 
       -----+---------+------+------+------+----------
          0 |       2 |    2 |    4 |    1 |        0
          1 |       2 |    5 |    8 |    1 |        1
          2 |       2 |    6 |    9 |    1 |        2
          3 |       2 |    9 |   16 |    1 |        3
          4 |       2 |    4 |    3 |    1 |        4
          5 |       2 |    3 |   -1 |    0 |        5
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

        seq | start_v | end_v | node | edge | cost | tot_cost 
       -----+---------+-------+------+------+------+----------
          0 |       2 |     3 |    2 |    4 |    1 |        0
          1 |       2 |     3 |    5 |    8 |    1 |        1
          2 |       2 |     3 |    6 |    9 |    1 |        2
          3 |       2 |     3 |    9 |   16 |    1 |        3
          4 |       2 |     3 |    4 |    3 |    1 |        4
          5 |       2 |     3 |    3 |   -1 |    0 |        5
       (6 rows)




Equivalences for :ref:`fig2-undirect-Cost-Reverse` 
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

        seq | node | edge | cost | tot_cost 
       -----+------+------+------+----------
          0 |    2 |    2 |    1 |        0
          1 |    3 |   -1 |    0 |        1
       (2 rows)



        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2, array[3],
                false     
        );
        seq | end_v | node | edge | cost | tot_cost 
       -----+-------+------+------+------+----------
          0 |     3 |    2 |    2 |    1 |        0
          1 |     3 |    3 |   -1 |    0 |        1
       (2 rows)


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], 3,
                false
        );
        seq | start_v | node | edge | cost | tot_cost 
       -----+---------+------+------+------+----------
          0 |       2 |    2 |    2 |    1 |        0
          1 |       2 |    3 |   -1 |    0 |        1
       (2 rows)


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], array[3],
                false
        );

        seq | start_v | end_v | node | edge | cost | tot_cost 
       -----+---------+-------+------+------+------+----------
          0 |       2 |     3 |    2 |    2 |    1 |        0
          1 |       2 |     3 |    3 |   -1 |    0 |        1
       (2 rows)


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
