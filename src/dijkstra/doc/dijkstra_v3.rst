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

Dijkstra's algorithm, conceived by Dutch computer scientist Edsger Dijkstra in 1956. It is a graph search algorithm that solves the single-source shortest path problem for a graph with non-negative edge path costs, producing a shortest path tree.


.. code-block:: sql

	SET OF (seq, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, bigint source, bigint target);

	SET OF (seq, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, bigint source, bigint target,
	                           boolean directed:=true);

	SET OF (seq, start_v, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, array[ANY_INTEGER] source, bigint target,
	                           boolean directed:=true);

	SET OF (seq, end_v, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, bigint source, array[ANY_INTEGER] target,
	                           boolean directed:=true);

	SET OF (seq, start_v, end_v, node, edge, cost, tot_cost)
	    pgr_dijkstra(text sql, array[ANY_INTEGER] source, array[ANY_INTEGER] target,
	                           boolean directed:=true);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost [,reverse_cost] FROM edge_table


	:id: ``ANY-INTEGER`` identifier of the edge
	:source: ``ANY-INTEGER`` identifier of the source vertex
	:target: ``ANY-INTEGER`` identifier of the target vertex
	:cost: ``ANY-NUMERICAL`` value of the edge traversal cost. A negative cost will prevent the edge (``source``, ``target``) from being inserted in the graph.
	:reverse_cost: ``ANY-NUMERICAL`` (optional) the value for the reverse traversal of the edge. A negative cost will prevent the edge (``target``, ``source``) from being inserted in the graph.

:start_v: ``BIGINT`` id of the starting vertex
:end_v: ``BIGINT`` id of the ending vertex
:directed: ``boolean`` (optional). When ``false`` the graph is considered as Undirected. Default is ``true`` which considers the graph as Directed.

Returns set of ``(seq, node, edge, cost, tot_cost)``

:seq:   row sequence
:node:   node ID
:edge:   edge ID (``-1`` for the last row)
:cost:  cost to traverse from ``node`` using ``edge`` to the next node in the sequence
:tot_cost:  total cost from ``start_v`` to this point in the route


.. rubric:: History

* Renamed in version 2.0.0 

then we can see in  :ref:`fig1-direct-Cost-Reverse`
then we can see in  :ref:`fig2-undirect-Cost-Reverse`
then we can see in  :ref:`fig3-direct-Cost`
then we can see in  :ref:`fig4-undirect-Cost`



Examples for :ref:`fig1-direct-Cost-Reverse` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5
                );

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5]
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5]
                );


Examples for :ref:`fig2-undirect-Cost-Reverse` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 3,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, 5,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 3,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        11, 5,
                        false
                );

       
        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2,11], 5,
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        2, array[3,5],
                        false
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost, reverse_cost FROM edge_table',
                        array[2, 11], array[3,5],
                        false
                );


Examples for :ref:`fig3-direct-Cost` 
-------------------------------------------------------------------------------

.. code-block:: sql

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 3
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        11, 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2,11], 5
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        2, array[3,5]
                );

        SELECT * FROM pgr_dijkstra(
                        'SELECT id, source, target, cost FROM edge_table',
                        array[2, 11], array[3,5]
                );




Examples for :ref:`fig4-undirect-Cost` 
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 3,
                        false,
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, 5,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 3,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			11, 5,
                        false
		);

       
	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2,11], 5,
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			2, array[3,5],
                        false
		);

	SELECT * FROM pgr_dijkstra(
			'SELECT id, source, target, cost FROM edge_table',
			array[2, 11], array[3,5],
                        false
		);




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


See Also
-------------------------------------------------------------------------------

* http://en.wikipedia.org/wiki/Dijkstra%27s_algorithm
