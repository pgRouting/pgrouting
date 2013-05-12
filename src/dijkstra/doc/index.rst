.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_dijkstra:

pgr_dijkstra — Shortest Path Dijkstra
===============================================================================

.. index:: 
	single: pgr_dijkstra(text,integer,integer,boolean,boolean)
	module: dijkstra

Name
-------------------------------------------------------------------------------

``pgr_dijkstra`` — Returns the shortest path using Dijkstra algorithm.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`path_result <type_path_result>` (vertex_id, edge_id, cost) rows, that make up a path.

.. code-block:: sql

	path_result[] pgr_dijkstra(text sql, integer source, integer target, 
	                           boolean directed, boolean reverse_cost);


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the directed and ``reverse_cost`` parameters are ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:target: ``int4`` id of the end point
:directed: ``true`` if the graph is directed
:reverse_cost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_dijkstra(
		'SELECT id, source, target, cost FROM edge_table',
		3, 7, false, false
	);


	 vertex_id | edge_id | cost 
	-----------+---------+------------------------
	         3 |       2 |    0.000763954363701041
	         4 |      21 |    0.00150254971056274
	         6 |       5 |    0.000417442425988342
	         7 |      -1 |    0
	(4 rows)


See Also
-------------------------------------------------------------------------------

* :ref:`type_path_result`
