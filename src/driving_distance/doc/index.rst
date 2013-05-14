.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_driving_distance:

pgr_drivingDistance - Driving Distance
===============================================================================

.. index:: 
	single: pgr_drivingDistance(text,integer,double precision,boolean,boolean)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_drivingDistance`` - Returns the driving distance from a start node.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`path_result <type_path_result>` (vertex_id, edge_id, cost) rows, that make up a path.

.. code-block:: sql

	path_result[] pgr_drivingDistance(text sql, integer source, double precision distance, 
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
:distance: ``float8`` value in edge cost units (not in projection units - they might be different).
:directed: ``true`` if the graph is directed
:reverse_cost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.


.. rubric:: History

* Renamed in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_drivingDistance(
		'SELECT id, source, target, cost FROM edge_table',
		<source id>, <cost as distance>, false, false
	);


See Also
-------------------------------------------------------------------------------

* :ref:`type_path_result`
