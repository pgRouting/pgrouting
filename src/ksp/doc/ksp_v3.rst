.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_ksp_v3:

pgr_ksp - K-Shortest Path
===============================================================================

.. index:: 
  single: pgr_ksp(text,integer,integer,integer,boolean)
  module: ksp

Name
-------------------------------------------------------------------------------

``pgr_ksp`` — Returns the "K" shortest paths.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside


Synopsis
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the number of shortest paths desired.

The minimal signature:

.. code-block:: sql

  pgr_ksp(TEXT sql_q, BIGINT start_v, BIGINT end_v, INTEGER k);
    RETURNS SET OF (seq, path_seq, route, node, edge, cost, agg_cost) or EMPTY SET

The full signature:

.. code-block:: sql

  pgr_ksp(TEXT sql_q, BIGINT start_v, BIGINT end_v, INTEGER k,
        BOOLEAN directed:=true, BOOLEAN heap_paths:=false);
    RETURNS SET OF (seq, path_seq, route, node, edge, cost, agg_cost) or EMPTY SET


Description of the SQL query
-------------------------------------------------------------------------------

General description of the ``sql_q``

.. code-block:: sql

    SELECT id, source, target, cost [,reverse_cost] FROM ...

:sql_q: a SQL query, which returns a set of rows with the following columns:

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

:sql_q: ``TEXT`` SQL query as decribed above.
:start_v: ``BIGINT`` id of the starting vertex.
:end_v: ``BIGINT`` id of the ending vertex.
:k: ``INTEGER`` The desiered number of paths.
:directed: ``BOLLEAN`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
:heap_paths: ``BOLLEAN`` (optional). When ``true`` returns all the paths stored in the process heap. Default is ``false`` which only returns ``k`` pahts. 

Roughly, if the shortest path has ``N`` edges, the heap will contain about than ``N * k`` paths for small value of ``k`` and ``k > 1``.



Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, route, node, edge, cost, agg_cost)``

:seq: ``INT`` sequential number starting from **1**.
:path_seq: ``INT`` relative position in the pathi of ``node`` and ``edge``. Has value **1** for the begining of a path.
:route: ``BIGINT`` route id. The ordering of the routes: For two routes i, j if i < j then agg_cost(i) <= tot_cost(j).
:node: ``BIGINT`` id of the node in the path from start_v to end_v.
:edge: ``BIGINT`` id of the edge used to go from ``node`` to the next node in the path sequence. ``-1`` for the last node of the route.
:cost: ``FLOAT`` cost to traverse from ``node`` using ``edge`` to the next node in the path sequence.
:agg_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.


.. warning:: During the transition to 3.0, because pgr_ksp version 2.0 doesn't have defined a directed flag nor a heap_path flag, when pgr_ksp is used with only one flag version 2.0 will be used.


Examples to handle the one flag to choose signatures
------------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`

.. code-block:: sql

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, 
      true   -- takes the (V2.0) signature (has_rcost = true and works on directed graph)
   );
   seq | id1 | id2 | id3 | cost 
  -----+-----+-----+-----+------
     0 |   0 |   2 |   4 |    1
     1 |   0 |   5 |   8 |    1
     2 |   0 |   6 |   9 |    1
     3 |   0 |   9 |  15 |    1
     4 |   0 |  12 |  -1 |    0
     5 |   1 |   2 |   4 |    1
     6 |   1 |   5 |   8 |    1
     7 |   1 |   6 |  11 |    1
     8 |   1 |  11 |  13 |    1
     9 |   1 |  12 |  -1 |    0
  (10 rows)


   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2,
      directed:=true   -- takes the new signature
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );

 	seq | path_seq | route | node | edge | cost | agg_cost 
	-----+----------+-------+------+------+------+----------
	   1 |        1 |     0 |    2 |    4 |    1 |        0
	   2 |        2 |     0 |    5 |    8 |    1 |        1
	   3 |        3 |     0 |    6 |    9 |    1 |        2
	   4 |        4 |     0 |    9 |   15 |    1 |        3
	   5 |        5 |     0 |   12 |   -1 |    0 |        4
	   6 |        1 |     1 |    2 |    4 |    1 |        0
	   7 |        2 |     1 |    5 |    8 |    1 |        1
	   8 |        3 |     1 |    6 |   11 |    1 |        2
	   9 |        4 |     1 |   11 |   13 |    1 |        3
	  10 |        5 |     1 |   12 |   -1 |    0 |        4
	(10 rows)


Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`


.. code-block:: sql

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
   (10 rows)
  

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

   SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, true, true
   );

    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
     11 |        1 |     2 |    2 |    4 |    1 |        0
     12 |        2 |     2 |    5 |   10 |    1 |        1
     13 |        3 |     2 |   10 |   12 |    1 |        2
     14 |        4 |     2 |   11 |   13 |    1 |        3
     15 |        5 |     2 |   12 |   -1 |    0 |        4
   (15 rows)


Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`


.. code-block:: sql

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, directed:=false
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    2 |    1 |        0
      2 |        2 |     0 |    3 |    3 |    1 |        1
      3 |        3 |     0 |    4 |   16 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |    9 |    1 |        2
      9 |        4 |     1 |    9 |   15 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
   (10 rows)



  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      2, 12, 2, false, true
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    2 |    1 |        0
      2 |        2 |     0 |    3 |    3 |    1 |        1
      3 |        3 |     0 |    4 |   16 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |    9 |    1 |        2
      9 |        4 |     1 |    9 |   15 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
     11 |        1 |     2 |    2 |    4 |    1 |        0
     12 |        2 |     2 |    5 |   10 |    1 |        1
     13 |        3 |     2 |   10 |   12 |    1 |        2
     14 |        4 |     2 |   11 |   13 |    1 |        3
     15 |        5 |     2 |   12 |   -1 |    0 |        4
     16 |        1 |     3 |    2 |    4 |    1 |        0
     17 |        2 |     3 |    5 |   10 |    1 |        1
     18 |        3 |     3 |   10 |   12 |    1 |        2
     19 |        4 |     3 |   11 |   11 |    1 |        3
     20 |        5 |     3 |    6 |    9 |    1 |        4
     21 |        6 |     3 |    9 |   15 |    1 |        5
     22 |        7 |     3 |   12 |   -1 |    0 |        6
   (22 rows)




Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`


Empty path representation

.. code-block:: sql

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 3, 2
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------

  (0 rows)


.. code-block:: sql

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
   (10 rows)



  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, true, true
   );
    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
     11 |        1 |     2 |    2 |    4 |    1 |        0
     12 |        2 |     2 |    5 |   10 |    1 |        1
     13 |        3 |     2 |   10 |   12 |    1 |        2
     14 |        4 |     2 |   11 |   13 |    1 |        3
     15 |        5 |     2 |   12 |   -1 |    0 |        4
   (15 rows)




Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`


.. code-block:: sql

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false
   );

    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
   (10 rows)

  
  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, directed:=false, heap_paths:=true
   );

  SELECT * FROM pgr_ksp(
     'SELECT id, source, target, cost FROM edge_table',
      2, 12, 2, false, true
   );

    seq | path_seq | route | node | edge | cost | agg_cost 
   -----+----------+-------+------+------+------+----------
      1 |        1 |     0 |    2 |    4 |    1 |        0
      2 |        2 |     0 |    5 |    8 |    1 |        1
      3 |        3 |     0 |    6 |    9 |    1 |        2
      4 |        4 |     0 |    9 |   15 |    1 |        3
      5 |        5 |     0 |   12 |   -1 |    0 |        4
      6 |        1 |     1 |    2 |    4 |    1 |        0
      7 |        2 |     1 |    5 |    8 |    1 |        1
      8 |        3 |     1 |    6 |   11 |    1 |        2
      9 |        4 |     1 |   11 |   13 |    1 |        3
     10 |        5 |     1 |   12 |   -1 |    0 |        4
     11 |        1 |     2 |    2 |    4 |    1 |        0
     12 |        2 |     2 |    5 |   10 |    1 |        1
     13 |        3 |     2 |   10 |   12 |    1 |        2
     14 |        4 |     2 |   11 |   13 |    1 |        3
     15 |        5 |     2 |   12 |   -1 |    0 |        4
   (15 rows)



The queries use the :ref:`sampledata` network.



.. rubric:: History

* New in version 2.0.0
* Added functionality for version 3.0.0 in version 2.1

See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/K_shortest_path_routing
