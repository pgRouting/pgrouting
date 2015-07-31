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
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956.
It is a graph search algorithm that solves the shortest path problem for
a graph with non-negative edge path costs, producing a shortest path from 
a starting vertex (``start_v``) to an ending vertex (``end_v``).
This implementation can be used with a directed graph and an undirected graph.

Signatures
===============================================================================

.. rubric:: Minimal signature

The minimal signature is for a **directed** graph from one ``start_v`` to one ``end_v``:

.. code-block:: sql

      pgr_dijkstra(text sql, bigint start_v, bigint end_v)
       	 RETURNS SET OF (seq, node, edge, cost, agg_cost) or EMPTY SET


.. rubric:: Dijkstra 1 to 1

This signature performs a Dijkstra from one ``start_v`` to one ``end_v``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

.. code-block:: sql

      pgr_dijkstra(text sql, bigint start_v, bigint end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, node, edge, cost, agg_cost) or EMPTY SET


.. rubric:: Dijkstra many to 1:

This signature performs a Dijkstra from many ``start_v`` to one ``end_v``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several 1 to 1 Dijkstra
where the ending vertex is fixed.

The extra ``start_v`` in the result is used to distinguish to which path it belongs.

.. code-block:: sql

      pgr_dijkstra(text sql, array[ANY_INTEGER] start_v, bigint end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, start_v, node, edge, cost, agg_cost) or EMPTY SET

.. rubric:: Dijkstra 1 to many:

This signature performs a Dijkstra from one ``start_v`` to many ``end_v``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.

Using this signature, will load once the graph and perform several 1 to 1 Dijkstra
where the starting vertex is fixed.

The extra ``end_v`` in the result is used to distinguish to which path it belongs.

.. code-block:: sql

       pgr_dijkstra(text sql, bigint start_v, array[ANY_INTEGER] end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, end_v, node, edge, cost, agg_cost) or EMPTY SET

.. rubric:: Dijkstra many to many:


This signature performs a Dijkstra from many ``start_v`` to many ``end_v``:
  -  on a **directed** graph when ``directed`` flag is missing or is set to ``true``.
  -  on an **undirected** graph when ``directed`` flag is set to ``false``.


Using this signature, will load once the graph and perform all combinations 
for starting vertices and ending vertices.

The extra ``start_v`` and ``end_v`` in the result is used to distinguish to which path it belongs.

.. code-block:: sql

       pgr_dijkstra(text sql, array[ANY_INTEGER] start_v, array[ANY_INTEGER] end_v,
	                 boolean directed:=true);
       	 RETURNS SET OF (seq, start_v, end_v, node, edge, cost, agg_cost) or EMPTY SET


The problem definition
======================

The graphs are defined as follows:

.. rubric:: Directed graph

The weighted directed graph, ``G_d(V,E)``, is definied by:

* the set of vertices 

  - ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``

* the set of edges

  - when ``reverse_cost`` column is *not* used: 

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``

  - when ``reverse_cost`` column is used: 

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, reverse_cost) where reverse_cost >=0)}``

**This is done transparently using directed Boost.Graph**

.. rubric:: Undirected graph

The weighted undirected graph, ``G_u(V,E)``, is definied by:

* the set of vertices

  -  ``V`` = ``source`` Union ``target`` Union ``{start_v}`` Union ``{end_v}``

* the set of edges

  - when ``reverse_cost`` column is *not* used:

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost) where cost >=0)}``


  - when ``reverse_cost`` column is used:

    - ``E`` = ``{ (source, target, cost) where cost >=0 }``  union ``{ (target, source, cost) where cost >=0)}``  \
      union ``{ (target, source, reverse_cost) where cost >=0 }``  union ``{ (source, target,  reverse_cost) where reverse_cost >=0)}``

**This is done transparently using directed Boost.Graph**

.. rubric:: The problem

Given a graph:

  - ``G(V,E)``  where ``G(V,E) = G_d(V,E)`` or ``G(V,E) = G_u(V,E)``

and the starting and ending vertices:
  - ``start_v`` and ``end_v``

The algorithm returns a path, if it exists, in terms of a sequence of vertices and of edges,
set of ``(seq, node, edge, cost, agg_cost)``
which is the shortest path using Dijsktra algorithm between ``start_v`` and ``end_v``,
where ``seq`` indicates the relative position in the path of the ``node`` or ``edge``.
  - When ``seq = 1`` then the row represents the begining of the path.
  - When ``edge = -1`` it represents the end of the path.
  - When ``node = end_v`` it represents the end of the path.


If there is no path, the resulting set is empty.

Aditional information like the cost (``cost``) of the edge to be used to go to the next node
and the aggregate cost (``agg_cost``) from the ``start_v`` up to the ``node`` is included.



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

    SELECT id, source, target, cost, reverse_cost from edge_table where geom && ST_Expand(ST_SetSRID(ST_Point(45, 34), 4326), 0.1)


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

Returns set of ``(seq [, start_v] [, end_v] , node, edge, cost, agg_cost)``

:seq: ``INT``  relative position in the path. Has value **1** for the begining of the path.
:start_v: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:end_v: ``BIGINT`` id of the ending vertex. Used when multiple ending vertices are in the query.
:node: ``BIGINT`` id of the node in the path from start_v to end_v.
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
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |    8 |    1 |        1
           3 |    6 |    9 |    1 |        2
           4 |    9 |   16 |    1 |        3
           5 |    4 |    3 |    1 |        4
           6 |    3 |   -1 |    0 |        5
        (6 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |   -1 |    0 |        1
        (2 rows)

When you pass an array we get a combined result:

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5]
                );
         seq | end_v | node | edge | cost | agg_cost 
        -----+-------+------+------+------+----------
           1 |     3 |    2 |    4 |    1 |        0
           2 |     3 |    5 |    8 |    1 |        1
           3 |     3 |    6 |    9 |    1 |        2
           4 |     3 |    9 |   16 |    1 |        3
           5 |     3 |    4 |    3 |    1 |        4
           6 |     3 |    3 |   -1 |    0 |        5
           1 |     5 |    2 |    4 |    1 |        0
           2 |     5 |    5 |   -1 |    0 |        1
        (8 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |   11 |   13 |    1 |        0
           2 |   12 |   15 |    1 |        1
           3 |    9 |   16 |    1 |        2
           4 |    4 |    3 |    1 |        3
           5 |    3 |   -1 |    0 |        4
        (5 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |   11 |   13 |    1 |        0
           2 |   12 |   15 |    1 |        1
           3 |    9 |    9 |    1 |        2
           4 |    6 |    8 |    1 |        3
           5 |    5 |   -1 |    0 |        4
        (5 rows)

Some other combinations.

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5
                );
         seq | start_v | node | edge | cost | agg_cost 
        -----+---------+------+------+------+----------
           1 |       2 |    2 |    4 |    1 |        0
           2 |       2 |    5 |   -1 |    0 |        1
           1 |      11 |   11 |   13 |    1 |        0
           2 |      11 |   12 |   15 |    1 |        1
           3 |      11 |    9 |    9 |    1 |        2
           4 |      11 |    6 |    8 |    1 |        3
           5 |      11 |    5 |   -1 |    0 |        4
        (7 rows)


        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5]
                );
         seq | start_v | end_v | node | edge | cost | agg_cost 
        -----+---------+-------+------+------+------+----------
           1 |       2 |     3 |    2 |    4 |    1 |        0
           2 |       2 |     3 |    5 |    8 |    1 |        1
           3 |       2 |     3 |    6 |    9 |    1 |        2
           4 |       2 |     3 |    9 |   16 |    1 |        3
           5 |       2 |     3 |    4 |    3 |    1 |        4
           6 |       2 |     3 |    3 |   -1 |    0 |        5
           1 |       2 |     5 |    2 |    4 |    1 |        0
           2 |       2 |     5 |    5 |   -1 |    0 |        1
           1 |      11 |     3 |   11 |   13 |    1 |        0
           2 |      11 |     3 |   12 |   15 |    1 |        1
           3 |      11 |     3 |    9 |   16 |    1 |        2
           4 |      11 |     3 |    4 |    3 |    1 |        3
           5 |      11 |     3 |    3 |   -1 |    0 |        4
           1 |      11 |     5 |   11 |   13 |    1 |        0
           2 |      11 |     5 |   12 |   15 |    1 |        1
           3 |      11 |     5 |    9 |    9 |    1 |        2
           4 |      11 |     5 |    6 |    8 |    1 |        3
           5 |      11 |     5 |    5 |   -1 |    0 |        4
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
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    2 |    1 |        0
           2 |    3 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5,
                        false
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3,
                        false
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |   11 |   11 |    1 |        0
           2 |    6 |    5 |    1 |        1
           3 |    3 |   -1 |    0 |        2
        (3 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5,
                        false
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |   11 |   11 |    1 |        0
           2 |    6 |    8 |    1 |        1
           3 |    5 |   -1 |    0 |        2
        (3 rows)

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5,
                        false
                );
         seq | start_v | node | edge | cost | agg_cost 
        -----+---------+------+------+------+----------
           1 |       2 |    2 |    4 |    1 |        0
           2 |       2 |    5 |   -1 |    0 |        1
           1 |      11 |   11 |   11 |    1 |        0
           2 |      11 |    6 |    8 |    1 |        1
           3 |      11 |    5 |   -1 |    0 |        2
        (5 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5],
                        false
                );
         seq | end_v | node | edge | cost | agg_cost 
        -----+-------+------+------+------+----------
           1 |     3 |    2 |    2 |    1 |        0
           2 |     3 |    3 |   -1 |    0 |        1
           1 |     5 |    2 |    4 |    1 |        0
           2 |     5 |    5 |   -1 |    0 |        1
        (4 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5],
                        false
                );
         seq | start_v | end_v | node | edge | cost | agg_cost 
        -----+---------+-------+------+------+------+----------
           1 |       2 |     3 |    2 |    2 |    1 |        0
           2 |       2 |     3 |    3 |   -1 |    0 |        1
           1 |       2 |     5 |    2 |    4 |    1 |        0
           2 |       2 |     5 |    5 |   -1 |    0 |        1
           1 |      11 |     3 |   11 |   11 |    1 |        0
           2 |      11 |     3 |    6 |    5 |    1 |        1
           3 |      11 |     3 |    3 |   -1 |    0 |        2
           1 |      11 |     5 |   11 |   11 |    1 |        0
           2 |      11 |     5 |    6 |    8 |    1 |        1
           3 |      11 |     5 |    5 |   -1 |    0 |        2
        (10 rows)
        

Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
        (0 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 5
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
           1 |    2 |    4 |    1 |        0
           2 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 3
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
        (0 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 5
                );
         seq | node | edge | cost | agg_cost 
        -----+------+------+------+----------
        (0 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2,11], 5
                );
         seq | start_v | node | edge | cost | agg_cost 
        -----+---------+------+------+------+----------
           1 |       2 |    2 |    4 |    1 |        0
           2 |       2 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, array[3,5]
                );
         seq | end_v | node | edge | cost | agg_cost 
        -----+-------+------+------+------+----------
           1 |     5 |    2 |    4 |    1 |        0
           2 |     5 |    5 |   -1 |    0 |        1
        (2 rows)

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2, 11], array[3,5]
                );
         seq | start_v | end_v | node | edge | cost | agg_cost 
        -----+---------+-------+------+------+------+----------
           1 |       2 |     5 |    2 |    4 |    1 |        0
           2 |       2 |     5 |    5 |   -1 |    0 |        1
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
        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    4 |    1 |        0
          2 |    5 |    8 |    1 |        1
          3 |    6 |    5 |    1 |        2
          4 |    3 |   -1 |    0 |        3
       (4 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 5,
                        false
		);
        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    4 |    1 |        0
          2 |    5 |   -1 |    0 |        1
       (2 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 3,
                        false
		);
        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |   11 |   11 |    1 |        0
          2 |    6 |    5 |    1 |        1
          3 |    3 |   -1 |    0 |        2
       (3 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 5,
                        false
		);
        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |   11 |   11 |    1 |        0
          2 |    6 |    8 |    1 |        1
          3 |    5 |   -1 |    0 |        2
       (3 rows)

       
	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2,11], 5,
                        false
		);
        seq | start_v | node | edge | cost | agg_cost 
       -----+---------+------+------+------+----------
          1 |       2 |    2 |    4 |    1 |        0
          2 |       2 |    5 |   -1 |    0 |        1
          1 |      11 |   11 |   11 |    1 |        0
          2 |      11 |    6 |    8 |    1 |        1
          3 |      11 |    5 |   -1 |    0 |        2
       (5 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, array[3,5],
                        false
		);
        seq | end_v | node | edge | cost | agg_cost 
       -----+-------+------+------+------+----------
          1 |     3 |    2 |    4 |    1 |        0
          2 |     3 |    5 |    8 |    1 |        1
          3 |     3 |    6 |    5 |    1 |        2
          4 |     3 |    3 |   -1 |    0 |        3
          1 |     5 |    2 |    4 |    1 |        0
          2 |     5 |    5 |   -1 |    0 |        1
       (6 rows)

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2, 11], array[3,5],
                        false
		);
        seq | start_v | end_v | node | edge | cost | agg_cost 
       -----+---------+-------+------+------+------+----------
          1 |       2 |     3 |    2 |    4 |    1 |        0
          2 |       2 |     3 |    5 |    8 |    1 |        1
          3 |       2 |     3 |    6 |    5 |    1 |        2
          4 |       2 |     3 |    3 |   -1 |    0 |        3
          1 |       2 |     5 |    2 |    4 |    1 |        0
          2 |       2 |     5 |    5 |   -1 |    0 |        1
          1 |      11 |     3 |   11 |   11 |    1 |        0
          2 |      11 |     3 |    6 |    5 |    1 |        1
          3 |      11 |     3 |    3 |   -1 |    0 |        2
          1 |      11 |     5 |   11 |   11 |    1 |        0
          2 |      11 |     5 |    6 |    8 |    1 |        1
          3 |      11 |     5 |    5 |   -1 |    0 |        2
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

       seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    4 |    1 |        0
          2 |    5 |    8 |    1 |        1
          3 |    6 |    9 |    1 |        2
          4 |    9 |   16 |    1 |        3
          5 |    4 |    3 |    1 |        4
          6 |    3 |   -1 |    0 |        5
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

       seq | start_v | node | edge | cost | agg_cost 
       -----+---------+------+------+------+----------
          1 |       2 |    2 |    4 |    1 |        0
          2 |       2 |    5 |    8 |    1 |        1
          3 |       2 |    6 |    9 |    1 |        2
          4 |       2 |    9 |   16 |    1 |        3
          5 |       2 |    4 |    3 |    1 |        4
          6 |       2 |    3 |   -1 |    0 |        5
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

        seq | start_v | end_v | node | edge | cost | agg_cost 
       -----+---------+-------+------+------+------+----------
          1 |       2 |     3 |    2 |    4 |    1 |        0
          2 |       2 |     3 |    5 |    8 |    1 |        1
          3 |       2 |     3 |    6 |    9 |    1 |        2
          4 |       2 |     3 |    9 |   16 |    1 |        3
          5 |       2 |     3 |    4 |    3 |    1 |        4
          6 |       2 |     3 |    3 |   -1 |    0 |        5
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

        seq | node | edge | cost | agg_cost 
       -----+------+------+------+----------
          1 |    2 |    2 |    1 |        0
          2 |    3 |   -1 |    0 |        1
       (2 rows)



        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                2, array[3],
                false     
        );
        seq | end_v | node | edge | cost | agg_cost 
       -----+-------+------+------+------+----------
          1 |     3 |    2 |    2 |    1 |        0
          2 |     3 |    3 |   -1 |    0 |        1
       (2 rows)


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], 3,
                false
        );
        seq | start_v | node | edge | cost | agg_cost 
       -----+---------+------+------+------+----------
          1 |       2 |    2 |    2 |    1 |        0
          2 |       2 |    3 |   -1 |    0 |        1
       (2 rows)


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], array[3],
                false
        );

        seq | start_v | end_v | node | edge | cost | agg_cost 
       -----+---------+-------+------+------+------+----------
          1 |       2 |     3 |    2 |    2 |    1 |        0
          2 |       2 |     3 |    3 |   -1 |    0 |        1
       (2 rows)


The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0 
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
