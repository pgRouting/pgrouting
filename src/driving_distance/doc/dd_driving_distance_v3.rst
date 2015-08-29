..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_driving_distance_v3:

pgr_drivingDistance
===============================================================================


Name
-------------------------------------------------------------------------------

``pgr_drivingDistance`` - Returns the driving distance from a start node.


.. figure:: ../../../doc/src/introduction/images/boost-inside.jpeg
   :target: http://www.boost.org/libs/graph

   Boost Graph Inside

Synopsis
-------------------------------------------------------------------------------

Using Dijkstra algorithm, extracts all the nodes that have costs less than or equal to the value ``distance``.
The edges extracted will conform the corresponding spanning tree.

.. index::
	single: drivingDistance(edges_sql, start_vid, distance)

.. rubric:: The minimal signature:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_v bigint, distance float8)
     RETURNS SET OF (seq, node, edge, cost, agg_cost)


.. index::
	single: drivingDistance(edges_sql, start_vid, distance, directed)

.. rubric:: Driving Distance from a single starting point:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_vid bigint, distance float8, directed boolean)
     RETURNS SET OF (seq, node, edge, cost, agg_cost)


.. index::
	single: drivingDistance(edges_sql, start_vids, distance, directed, equiCost)

.. rubric:: Driving Distance from a multiple starting points:

.. code-block:: sql

   pgr_drivingDistance(sql text, start_vids anyarray, distance float8,
         directed boolean default true,
         equicost boolean default false)
     RETURNS SET OF (seq, start_vid, node, edge, cost, agg_cost)

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
:distance: ``FLOAT`` Upper limit for the inclusion of the node in the result.
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.
:equicost: ``boolean`` (optional). When ``true`` the node will only appear in the closest ``start_v`` list.  Default is ``false`` which resembles several calls using the single starting point signatures. Tie brakes are arbitrarely.


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq [, start_v], node, edge, cost, agg_cost)``

:seq: ``INT`` row sequence.
:start_v: ``BIGINT`` id of the starting vertex. Used when multiple starting vetrices are in the query.
:node: ``BIGINT`` id of the node within the limits from ``start_v``.
:edge: ``BIGINT`` id of the edge used to arrive to ``node``. ``0`` when the ``node`` is the ``start_v``.
:cost: ``FLOAT`` cost to traverse ``edge``.
:agg_cost:  ``FLOAT`` total cost from ``start_v`` to ``node``.



Examples for queries marked as ``directed`` with ``cost`` and ``reverse_cost`` columns
--------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig1`


.. code-block:: sql

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    1 |    1 |    1 |        1
       1 |    2 |   -1 |    0 |        0
       2 |    5 |    4 |    1 |        1
       3 |    6 |    8 |    1 |        2
       4 |   11 |   12 |    1 |        3
       5 |   10 |   10 |    1 |        2
       6 |   13 |   14 |    1 |        3
       7 |    9 |    9 |    1 |        3
       8 |    7 |    6 |    1 |        3
       9 |    8 |    7 |    1 |        2
    (10 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    2 |    4 |    1 |        3
       1 |    5 |   10 |    1 |        2
       2 |    6 |    8 |    1 |        3
       3 |   11 |   12 |    1 |        2
       4 |   10 |   14 |    1 |        1
       5 |   12 |   13 |    1 |        3
       6 |   13 |   -1 |    0 |        0
       7 |    8 |    7 |    1 |        3
    (8 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+---------+------+------+------+----------
       0 |       2 |    1 |    1 |    1 |        1
       1 |       2 |    2 |   -1 |    0 |        0
       2 |       2 |    5 |    4 |    1 |        1
       3 |       2 |    6 |    8 |    1 |        2
       4 |       2 |   11 |   12 |    1 |        3
       5 |       2 |   10 |   10 |    1 |        2
       6 |       2 |   13 |   14 |    1 |        3
       7 |       2 |    9 |    9 |    1 |        3
       8 |       2 |    7 |    6 |    1 |        3
       9 |       2 |    8 |    7 |    1 |        2
      10 |      13 |    2 |    4 |    1 |        3
      11 |      13 |    5 |   10 |    1 |        2
      12 |      13 |    6 |    8 |    1 |        3
      13 |      13 |   11 |   12 |    1 |        2
      14 |      13 |   10 |   14 |    1 |        1
      15 |      13 |   12 |   13 |    1 |        3
      16 |      13 |   13 |   -1 |    0 |        0
      17 |      13 |    8 |    7 |    1 |        3
    (18 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, equicost:=true
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+---------+------+------+------+----------
       0 |       2 |    1 |    1 |    1 |        1
       1 |       2 |    2 |   -1 |    0 |        0
       2 |       2 |    5 |    4 |    1 |        1
       3 |       2 |    6 |    8 |    1 |        2
       4 |       2 |    7 |    6 |    1 |        3
       5 |       2 |    8 |    7 |    1 |        2
       6 |       2 |    9 |    9 |    1 |        3
       7 |       2 |   10 |   10 |    1 |        2
       8 |       2 |   11 |   12 |    1 |        3
       9 |      13 |   13 |   -1 |    0 |        0
      10 |      13 |   12 |   13 |    1 |        3
    (11 rows)



Examples for queries marked as ``undirected`` with ``cost`` and ``reverse_cost`` columns
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig2`


.. code-block:: sql

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        2, 3, false
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    1 |    1 |    1 |        1
       1 |    2 |   -1 |    0 |        0
       2 |    3 |    2 |    1 |        1
       3 |    4 |    3 |    1 |        2
       4 |    5 |    4 |    1 |        1
       5 |    6 |    8 |    1 |        2
       6 |   11 |   12 |    1 |        3
       7 |   10 |   10 |    1 |        2
       8 |   13 |   14 |    1 |        3
       9 |    9 |   16 |    1 |        3
      10 |    7 |    6 |    1 |        3
      11 |    8 |    7 |    1 |        2
    (12 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        13, 3, false
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    2 |    4 |    1 |        3
       1 |    5 |   10 |    1 |        2
       2 |    6 |   11 |    1 |        3
       3 |   11 |   12 |    1 |        2
       4 |   10 |   14 |    1 |        1
       5 |   12 |   13 |    1 |        3
       6 |   13 |   -1 |    0 |        0
       7 |    8 |    7 |    1 |        3
    (8 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    1 |    1 |    1 |        1
       1 |         2 |    2 |   -1 |    0 |        0
       2 |         2 |    3 |    2 |    1 |        1
       3 |         2 |    4 |    3 |    1 |        2
       4 |         2 |    5 |    4 |    1 |        1
       5 |         2 |    6 |    8 |    1 |        2
       6 |         2 |   11 |   12 |    1 |        3
       7 |         2 |   10 |   10 |    1 |        2
       8 |         2 |   13 |   14 |    1 |        3
       9 |         2 |    9 |   16 |    1 |        3
      10 |         2 |    7 |    6 |    1 |        3
      11 |         2 |    8 |    7 |    1 |        2
      12 |        13 |    2 |    4 |    1 |        3
      13 |        13 |    5 |   10 |    1 |        2
      14 |        13 |    6 |   11 |    1 |        3
      15 |        13 |   11 |   12 |    1 |        2
      16 |        13 |   10 |   14 |    1 |        1
      17 |        13 |   12 |   13 |    1 |        3
      18 |        13 |   13 |   -1 |    0 |        0
      19 |        13 |    8 |    7 |    1 |        3
    (20 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    1 |    1 |    1 |        1
       1 |         2 |    2 |   -1 |    0 |        0
       2 |         2 |    3 |    2 |    1 |        1
       3 |         2 |    4 |    3 |    1 |        2
       4 |         2 |    5 |    4 |    1 |        1
       5 |         2 |    6 |    8 |    1 |        2
       6 |         2 |    7 |    6 |    1 |        3
       7 |         2 |    8 |    7 |    1 |        2
       8 |         2 |    9 |   16 |    1 |        3
       9 |         2 |   10 |   10 |    1 |        2
      10 |         2 |   11 |   12 |    1 |        3
      11 |        13 |   13 |   -1 |    0 |        0
      12 |        13 |   12 |   13 |    1 |        3
    (13 rows)




Examples for queries marked as ``directed`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig3`


.. code-block:: sql

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    2 |   -1 |    0 |        0
       1 |    5 |    4 |    1 |        1
       2 |    6 |    8 |    1 |        2
       3 |   11 |   11 |    1 |        3
       4 |   10 |   10 |    1 |        2
       5 |   13 |   14 |    1 |        3
       6 |    9 |    9 |    1 |        3
    (7 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |   13 |   -1 |    0 |        0
    (1 row)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    2 |   -1 |    0 |        0
       1 |         2 |    5 |    4 |    1 |        1
       2 |         2 |    6 |    8 |    1 |        2
       3 |         2 |   11 |   11 |    1 |        3
       4 |         2 |   10 |   10 |    1 |        2
       5 |         2 |   13 |   14 |    1 |        3
       6 |         2 |    9 |    9 |    1 |        3
       7 |        13 |   13 |   -1 |    0 |        0
    (8 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, equicost:=true
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    2 |   -1 |    0 |        0
       1 |         2 |    5 |    4 |    1 |        1
       2 |         2 |    6 |    8 |    1 |        2
       3 |         2 |    9 |    9 |    1 |        3
       4 |         2 |   10 |   10 |    1 |        2
       5 |         2 |   11 |   11 |    1 |        3
       6 |        13 |   13 |   -1 |    0 |        0
    (7 rows)



Examples for queries marked as ``undirected`` with ``cost`` column
----------------------------------------------------------------------------------------

The examples in this section use the following :ref:`fig4`


.. code-block:: sql

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        2, 3, false
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    1 |    1 |    1 |        1
       1 |    2 |   -1 |    0 |        0
       2 |    3 |    5 |    1 |        3
       3 |    5 |    4 |    1 |        1
       4 |    6 |    8 |    1 |        2
       5 |   11 |   12 |    1 |        3
       6 |   10 |   10 |    1 |        2
       7 |   13 |   14 |    1 |        3
       8 |    9 |    9 |    1 |        3
       9 |    7 |    6 |    1 |        3
      10 |    8 |    7 |    1 |        2
    (11 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        13, 3, false
      );
     seq | node | edge | cost | agg_cost
    -----+------+------+------+----------
       0 |    2 |    4 |    1 |        3
       1 |    5 |   10 |    1 |        2
       2 |    6 |   11 |    1 |        3
       3 |   11 |   12 |    1 |        2
       4 |   10 |   14 |    1 |        1
       5 |   12 |   13 |    1 |        3
       6 |   13 |   -1 |    0 |        0
       7 |    8 |    7 |    1 |        3
    (8 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    1 |    1 |    1 |        1
       1 |         2 |    2 |   -1 |    0 |        0
       2 |         2 |    3 |    5 |    1 |        3
       3 |         2 |    5 |    4 |    1 |        1
       4 |         2 |    6 |    8 |    1 |        2
       5 |         2 |   11 |   12 |    1 |        3
       6 |         2 |   10 |   10 |    1 |        2
       7 |         2 |   13 |   14 |    1 |        3
       8 |         2 |    9 |    9 |    1 |        3
       9 |         2 |    7 |    6 |    1 |        3
      10 |         2 |    8 |    7 |    1 |        2
      11 |        13 |    2 |    4 |    1 |        3
      12 |        13 |    5 |   10 |    1 |        2
      13 |        13 |    6 |   11 |    1 |        3
      14 |        13 |   11 |   12 |    1 |        2
      15 |        13 |   10 |   14 |    1 |        1
      16 |        13 |   12 |   13 |    1 |        3
      17 |        13 |   13 |   -1 |    0 |        0
      18 |        13 |    8 |    7 |    1 |        3
    (19 rows)

    SELECT * FROM pgr_drivingDistance(
        'SELECT id, source, target, cost FROM edge_table',
        array[2,13], 3, false, equicost:=true
      );
     seq | start_vid | node | edge | cost | agg_cost
    -----+-----------+------+------+------+----------
       0 |         2 |    1 |    1 |    1 |        1
       1 |         2 |    2 |   -1 |    0 |        0
       2 |         2 |    3 |    5 |    1 |        3
       3 |         2 |    5 |    4 |    1 |        1
       4 |         2 |    6 |    8 |    1 |        2
       5 |         2 |    7 |    6 |    1 |        3
       6 |         2 |    8 |    7 |    1 |        2
       7 |         2 |    9 |    9 |    1 |        3
       8 |         2 |   10 |   10 |    1 |        2
       9 |         2 |   11 |   12 |    1 |        3
      10 |        13 |   13 |   -1 |    0 |        0
      11 |        13 |   12 |   13 |    1 |        3
    (12 rows)



The queries use the :ref:`sampledata` network.



.. rubric:: History

* Renamed in version 2.0.0
* Added functionality for version 3.0.0 in version 2.1


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_alphashape` - Alpha shape computation
* :ref:`pgr_points_as_polygon` - Polygon around set of points

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`

