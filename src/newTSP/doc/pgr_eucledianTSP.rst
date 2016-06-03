.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_eucledianTSP:

pgr_eucledianTSP
=============================================================================

Name
-------------------------------------------------------------------------------

* ``pgr_eucledianTSP`` - Returns a route that visits all the points exacly once.

Synopsis
-------------------------------------------------------------------------------

The travelling salesman problem (TSP) or travelling salesperson problem asks the following question:

  - Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city?
   
This implementation uses simulated annealing to return the approximate solution when the input is given in the form of coordinates.

Signature Summary
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_eucledianTSP(coordinates_sql)
    pgr_eucledianTSP(coordinates_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURS SETOF (seq, node, cost, agg_cost)


Signatures
===============

.. index::
    single: pgr_eucledianTSP(coordinates_sql) -- proposed

Minimal Signature
-------------------------------------------------------------------------------

.. code-block:: none

    pgr_eucledianTSP(coordinates_sql)
    RETURS SETOF (seq, node, cost, agg_cost)

:Example: 

Because the documentation examples are auto generated and tested for non changing results,
and the default is to have random execution, the example is wrapping the actual call.

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q1
   :end-before: -- q2


.. index::
    single: pgr_eucledianTSP(coordinates_sql, <configuration parameters>) -- proposed

Complete Signature
-------------------------------------------------------------------------------

.. code-block:: none


    pgr_eucledianTSP(coordinates_sql,
        start_id, end_id,
        max_processing_time,
        tries_per_temperature, max_changes_per_temperature, max_consecutive_non_changes,
        initial_temperature, final_temperature, cooling_factor,
        randomize,
    RETURS SETOF (seq, node, cost, agg_cost)


.. rubric:: Example:

.. literalinclude:: doc-pgr_eucledianTSP.queries
   :start-after: -- q2
   :end-before: -- q3

Description of the Signatures
=============================

Description of the coordinates SQL query
-------------------------------------------------------------------------------

======= =========== =================================================
Column  Type              Description
======= =========== =================================================
**id**  ``BIGINT``  Identifier of the coordinate. (optional)
**x**   ``FLOAT``   X value of the coordinate.
**y**   ``FLOAT``   Y value of the coordinate.
======= =========== =================================================


When the value of **id** is not given then the coordinates will receive an **id** starting from 1, in the order given.

..
    description of the configuration parameters


.. include:: tsp_parameters.txt


Description of the return values
-------------------------------------------------------------------------------

Returns set of ``(seq, node, cost, agg_cost)``

============= =========== =================================================
Column           Type              Description
============= =========== =================================================
**seq**       ``INTEGER`` Row sequence.
**node**      ``BIGINT``  Identifier of the coordinate.
**cost**      ``FLOAT``   Cost to traverse from the previous ``node`` the current ``node`` in the path sequence.
                            - ``0`` for the first row in the path sequence.

**agg_cost**  ``FLOAT``   Aggregate cost from the ``node`` at ``seq = 1`` to the current node.
                            - ``0`` for the first row in the path sequence.

============= =========== =================================================




.. rubric:: History

* New in version 2.3.0


Examples
-------------------------------------------------------------------------------

* Using SQL parameter (all points from the table, atarting from 6 and ending at 5). We have listed two queries in this example, the first might vary from system to system because there are multiple equivalent answers. The second query should be stable in that the length optimal route should be the same regardless of order.



.. code-block:: sql

    CREATE TABLE vertex_table (
        id serial,
        x double precision,
        y double precision
    );

    INSERT INTO vertex_table VALUES
    (1,2,0), (2,2,1), (3,3,1), (4,4,1), (5,0,2), (6,1,2), (7,2,2),
    (8,3,2), (9,4,2), (10,2,3), (11,3,3), (12,4,3), (13,2,4);

    SELECT seq, id1, id2, round(cost::numeric, 2) AS cost
      FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);

     seq | id1 | id2 | cost
    -----+-----+-----+------
       0 |   5 |   6 | 1.00
       1 |   6 |   7 | 1.00
       2 |   7 |   8 | 1.41
       3 |   1 |   2 | 1.00
       4 |   0 |   1 | 1.41
       5 |   2 |   3 | 1.00
       6 |   3 |   4 | 1.00
       7 |   8 |   9 | 1.00
       8 |  11 |  12 | 1.00
       9 |  10 |  11 | 1.41
      10 |  12 |  13 | 1.00
      11 |   9 |  10 | 2.24
      12 |   4 |   5 | 1.00
    (13 rows)

    SELECT round(sum(cost)::numeric, 4) as cost
   FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);

      cost
    ---------
     15.4787
    (1 row)



* Using distance matrix (A loop starting from 1)

When using just the start node you are getting a loop that starts with 1, in this case, and travels through the other nodes and is implied to return to the start node from the last one in the list. Since this is a circle there are at least two possible paths, one clockwise and one counter-clockwise that will have the same length and be equall valid. So in the following example it is also possible to get back a sequence of ids = {1,0,3,2} instead of the {1,2,3,0} sequence listed below.

.. code-block:: sql

	SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1);

	 seq | id 
	-----+----
	   0 |  1
	   1 |  2
	   2 |  3
	   3 |  0
	(4 rows)

* Using distance matrix (Starting from 1, ending at 2)

.. code-block:: sql

	SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,4,5},{2,4,0,6},{3,5,6,0}}'::float8[],1,2);

	 seq | id 
	-----+----
	   0 |  1
	   1 |  0
	   2 |  3
	   3 |  2
	(4 rows)

* Using the vertices table edge_table_vertices_pgr generated by :ref:`pgr_create_topology`. Again we have two queries where the first might vary and the second is based on the overal path length.

.. code-block:: sql

	SELECT seq, id1, id2, round(cost::numeric, 2) AS cost
   FROM pgr_tsp('SELECT id::integer, st_x(the_geom) as x,st_x(the_geom) as y FROM edge_table_vertices_pgr  ORDER BY id', 6, 5);

     seq | id1 | id2 | cost
    -----+-----+-----+------
       0 |   5 |   6 | 0.00
       1 |  10 |  11 | 0.00
       2 |   2 |   3 | 1.41
       3 |   3 |   4 | 0.00
       4 |  11 |  12 | 0.00
       5 |   8 |   9 | 0.71
       6 |  15 |  16 | 0.00
       7 |  16 |  17 | 2.12
       8 |   1 |   2 | 0.00
       9 |  14 |  15 | 1.41
      10 |   7 |   8 | 1.41
      11 |   6 |   7 | 0.71
      12 |  13 |  14 | 2.12
      13 |   0 |   1 | 0.00
      14 |   9 |  10 | 0.00
      15 |  12 |  13 | 0.00
      16 |   4 |   5 | 1.41
    (17 rows)

    SELECT round(sum(cost)::numeric, 4) as cost
       FROM pgr_tsp('SELECT id::integer, st_x(the_geom) as x,st_x(the_geom) as y FROM edge_table_vertices_pgr  ORDER BY id', 6, 5);

      cost
    ---------
     11.3137
    (1 row)


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Traveling_salesman_problem
* http://en.wikipedia.org/wiki/Simulated_annealing
