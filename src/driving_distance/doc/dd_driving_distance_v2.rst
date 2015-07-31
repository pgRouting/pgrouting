..
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_driving_distance_v2:

pgr_drivingDistance (V 2.0)
===============================================================================

.. index::
	single: pgr_drivingDistance(text,integer,double precision,boolean,boolean)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_drivingDistance`` - Returns the driving distance from a start node.


Synopsis
-------------------------------------------------------------------------------

This function computes a Dijkstra shortest path solution them extracts the cost to get to each node in the network from the starting node. Using these nodes and costs it is possible to compute constant drive time polygons. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a list of accessible points.

.. code-block:: sql

	pgr_costResult[] pgr_drivingDistance(text sql, integer source, double precision distance,
                                      boolean directed, boolean has_rcost);

.. warning:: This signature will be discontinued on version 3.0, Please use it
             without the ``has_rcost`` flag instead:

             ``pgr_dijkstra(text sql, integer source, integer target, boolean directed)``

             See :ref:`pgr_dijkstra_v3`


Description
-------------------------------------------------------------------------------

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, source, target, cost [,reverse_cost] FROM edge_table


	:id: ``int4`` identifier of the edge
	:source: ``int4`` identifier of the source vertex
	:target: ``int4`` identifier of the target vertex
	:cost: ``float8`` value, of the edge traversal cost. A negative cost will prevent the edge from being inserted in the graph.
	:reverse_cost: (optional) the cost for the reverse traversal of the edge. This is only used when the ``directed`` and ``has_rcost`` parameters are ``true`` (see the above remark about negative costs).

:source: ``int4`` id of the start point
:distance: ``float8`` value in edge cost units (not in projection units - they might be different).
:directed: ``true`` if the graph is directed
:has_rcost: if ``true``, the ``reverse_cost`` column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   node ID
:id2:   edge ID (this is probably not a useful item)
:cost:  cost to get to this node ID




Examples
-------------------------------------------------------------------------------

* Without ``reverse_cost``

.. code-block:: sql

	SELECT seq, id1 AS node, cost
		FROM pgr_drivingDistance(
			'SELECT id, source, target, cost FROM edge_table',
			6, 1.5, false, false
		);
         seq | node | cost
        -----+------+------
           0 |    3 |    1
           1 |    5 |    1
           2 |    6 |    0
           3 |   11 |    1
           4 |    9 |    1
        (5 rows)

* With ``reverse_cost``

.. code-block:: sql

	SELECT seq, id1 AS node, cost
		FROM pgr_drivingDistance(
			'SELECT id, source, target, cost, reverse_cost FROM edge_table',
			7, 1.5, true, true
		);
         seq | node | cost
        -----+------+------
           0 |    7 |    1
           1 |    8 |    0
        (2 rows)



The queries use the :ref:`sampledata` network.

.. rubric:: History

* Renamed in version 2.0.0


See Also
-------------------------------------------------------------------------------

* :ref:`pgr_alphashape` - Alpha shape computation
* :ref:`pgr_points_as_polygon` - Polygon around set of points
