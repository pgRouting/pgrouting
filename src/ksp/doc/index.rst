.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _ksp:

pgr_ksp - K-Shortest Path
===============================================================================

.. index:: 
  single: pgr_ksp(text,integer,integer,integer,boolean)
  module: ksp

Name
-------------------------------------------------------------------------------

``pgr_ksp`` — Returns the "K" shortest paths.


Synopsis
-------------------------------------------------------------------------------

The K shortest path routing algorithm based on Yen's algorithm. "K" is the number of shortest paths desired. Returns a set of :ref:`pgr_costResult3 <type_cost_result3>` (seq, id1, id2, id3, cost) rows, that make up a path.

.. code-block:: sql

  pgr_costResult3[] pgr_ksp(sql text, source integer, target integer,
                           paths integer, has_rcost boolean);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

  .. code-block:: sql

    SELECT id, source, target, cost, [,reverse_cost] FROM edge_table


  :id: ``int4`` identifier of the edge
  :source: ``int4`` identifier of the source vertex
  :target: ``int4`` identifier of the target vertex
  :cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
  :reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when ``has_rcost`` the parameter is ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:target: ``int4`` id of the end point
:paths: ``int4`` number of alternative routes
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   sequence for ording the results
:id1:   route ID
:id2:   node ID
:id3:   edge ID (``0`` for the last row)
:cost:  cost to traverse from ``id2`` using ``id3``

KSP code base taken from http://code.google.com/p/k-shortest-paths/source.


.. rubric:: History

* New in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. code-block:: sql

   SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
      'SELECT id, source, target, cost FROM edge_table',
      7, 12, 2, false
    );

    seq | route | node | edge | cost 
   -----+-------+------+------+------
      0 |     0 |    7 |    6 |    1
      1 |     0 |    8 |    7 |    1
      2 |     0 |    5 |    8 |    1
      3 |     0 |    6 |   11 |    1
      4 |     0 |   11 |   13 |    1
      5 |     0 |   12 |    0 |    0
      6 |     1 |    7 |    6 |    1
      7 |     1 |    8 |    7 |    1
      8 |     1 |    5 |    8 |    1
      9 |     1 |    6 |    9 |    1
     10 |     1 |    9 |   15 |    1
     11 |     1 |   12 |    0 |    0
   (12 rows)



* With ``reverse_cost``

.. code-block:: sql

   SELECT seq, id1 AS route, id2 AS node, id3 AS edge, cost
    FROM pgr_ksp(
      'SELECT id, source, target, cost, reverse_cost FROM edge_table',
      7, 12, 2, true
    );

    seq | route | node | edge | cost 
   -----+-------+------+------+------
      0 |     0 |    7 |    6 |    1
      1 |     0 |    8 |    7 |    1
      2 |     0 |    5 |    8 |    1
      3 |     0 |    6 |   11 |    1
      4 |     0 |   11 |   13 |    1
      5 |     0 |   12 |    0 |    0
      6 |     1 |    7 |    6 |    1
      7 |     1 |    8 |    7 |    1
      8 |     1 |    5 |    8 |    1
      9 |     1 |    6 |    9 |    1
     10 |     1 |    9 |   15 |    1
     11 |     1 |   12 |    0 |    0
   (12 rows)


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result3`
* http://en.wikipedia.org/wiki/K_shortest_path_routing
