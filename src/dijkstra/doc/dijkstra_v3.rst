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

``pgr_dijkstra`` — Returns the shortest path form a single source to a single target using Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956. It is a graph search algorithm that solves the single-source shortest path problem for a graph with non-negative edge path costs, producing a shortest path tree. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_dijkstra(text sql, integer source, integer target);

	pgr_costResult[] pgr_dijkstra(text sql, integer source, integer target,
	                           boolean directed);

	pgr_costResult[] pgr_dijkstra(text sql, integer source, integer target,
	                           boolean directed, boolean has_rcost);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
	:reverse_cost: ``float8`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``. ``source``) from being inserted in the graph.

:starting: ``int4`` id of the starting vertex
:ending: ``int4`` id of the ending vertex
:directed: ``boolean`` (optional). When ``true`` the graph is considered as Directed. Default is ``false`` which considers the graph as Undirected.
:has_rcost: ``boolean`` (optional). Default is auto detected. If set to ``true``, the ``reverse_cost`` column will be used (error if it doesn't exist). If set to ``false`` the ``reverse_cost`` column will be not be used even if it exist (but a message will show)

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``id1`` using ``id2``


.. rubric:: History

* Renamed in version 2.0.0 


Examples for Undirected Graph
-------------------------------------------------------------------------------

* Without ``reverse_cost``

The following queries are equivalent

.. code-block:: sql

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false,    -- directed flag
                        false     -- has_rcost
		);

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false     -- directed flag
			          -- has_rcost is set to false automatically
		);


	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3
                                  -- directed's flag default is false
			          -- has_rcost is set to false automatically
		);

         seq | node | edge | cost 
        -----+------+------+------
           0 |    2 |    4 |    1
           1 |    5 |    8 |    1
           2 |    6 |    5 |    1
           3 |    3 |   -1 |    0
        (4 rows)



* With ``reverse_cost``

The following queries  

Examples:  Equivalences for directed Graph
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
          0 |    2 |    4 |    1 |        1
          1 |    5 |    8 |    1 |        2
          2 |    6 |    9 |    1 |        3
          3 |    9 |   16 |    1 |        4
          4 |    4 |    3 |    1 |        5
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


       seq | end_v | node | edge | cost | tot_cost 
       -----+-------+------+------+------+----------
          0 |     3 |    2 |    4 |    1 |        1
          1 |     3 |    5 |    8 |    1 |        2
          2 |     3 |    6 |    9 |    1 |        3
          3 |     3 |    9 |   16 |    1 |        4
          4 |     3 |    4 |    3 |    1 |        5
          5 |     3 |    3 |   -1 |    0 |        5
       (6 rows)


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], 3,
                true
        );


        SELECT * FROM pgr_dijkstra(
                'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                array[2], 3
        );


       seq | start_v | node | edge | cost | tot_cost 
       -----+---------+------+------+------+----------
          0 |       2 |    2 |    4 |    1 |        1
          1 |       2 |    5 |    8 |    1 |        2
          2 |       2 |    6 |    9 |    1 |        3
          3 |       2 |    9 |   16 |    1 |        4
          4 |       2 |    4 |    3 |    1 |        5
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
          0 |       2 |     3 |    2 |    4 |    1 |        1
          1 |       2 |     3 |    5 |    8 |    1 |        2
          2 |       2 |     3 |    6 |    9 |    1 |        3
          3 |       2 |     3 |    9 |   16 |    1 |        4
          4 |       2 |     3 |    4 |    3 |    1 |        5
          5 |       2 |     3 |    3 |   -1 |    0 |        5
       (6 rows)
       



The queries use the :ref:`sampledata` network.


Examples for Directed Graph
-------------------------------------------------------------------------------

* Without ``reverse_cost``

The following queries are equivalent

.. code-block:: sql

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        true,    -- directed flag
                        false    -- has_rcost
		);

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        true     -- directed flag
                                 -- has_rcost is set to false automatically
		);

         seq | node | edge | cost 
        -----+------+------+------
           0 |    2 |   -1 |    0
        (1 rows)



* With ``reverse_cost``

The following queries are equivalent

.. code-block:: sql

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost, reverse_cost FROM edge_table',
			2, 3,
                        true,    -- directed flag
                        true     -- has_rcost
		);

	SELECT seq, id1 AS node, id2 AS edge, cost 
		FROM pgr_dijkstra(
			'SELECT id, source, target, cost, reverse_cost FROM edge_table',
			2, 3,
                        true     -- directed flag
			         -- has_rcost is set to true automatically
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


The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
