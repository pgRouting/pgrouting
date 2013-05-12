.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This work is licensed under a Creative Commons Attribution-Share Alike 3.0 
    License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_tsp:

pgr_tsp — Traveling Sales Person
===============================================================================

.. index:: 
	single: pgr_tsp(text,integer,double precision,boolean,boolean)
	module: driving_distance

Name
-------------------------------------------------------------------------------

``pgr_tsp`` — Returns the best route from a start node via a list of nodes.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`path_result <type_path_result>` (vertex_id, edge_id, cost) rows, that make up a path.

.. code-block:: sql

	path_result[] pgr_tsp(text sql, varchar ids, integer source);


Description
-------------------------------------------------------------------------------

The tsp solution is based on ordering the points using straight line (euclidean) distance between nodes. There was some thought given to pre-calculating the driving distances between the nodes using Dijkstra, but then I read a paper (unfortunately I don't remember who wrote it), where it was proved that the quality of TSP with euclidean distance is only slightly worse that one with real distance in case of normal city layout. In case of very sparse network or rivers and bridges it becomes more inaccurate, but still wholly satisfactory. Of course it is nice to have exact solution, but this is a compromise between quality and speed (and development time also).

The TSP solver is using a genetic algorithm. It is not an exact solution, but it is guarantied that you get a solution after certain number of iterations.

:sql: a SQL query, which should return a set of rows with the following columns:

	:source_id: ``int4`` identifier of the vertex
	:x: ``x`` coordinate of the vertex
	:y: ``y`` coordinate of the vertex

:ids: ``text`` string containig ``int4`` ids of vertices separated by commas
:source: ``int4`` id of the start point

.. note::
	``edge_id`` and ``cost`` attribute of the result set are not used and always contain ``0``.


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_tsp('SELECT distinct source AS source_id, 
		               x1::double precision AS x, 
		               y1::double precision AS y FROM dourol 
		     WHERE source IN (83593,66059,10549,18842,13)',
		               '83593,66059,10549,18842,13', 10549);


.. code-block:: sql

	 vertex_id | edge_id | cost
	-----------+---------+------
		 10549 |       0 |    0
		 83593 |       0 |    0
		 66059 |       0 |    0
		 18842 |       0 |    0
		    13 |       0 |    0
	(5 rows)
	
.. note::
	Afterwards ``vertex_id`` column can be used for shortest path calculation.


See Also
-------------------------------------------------------------------------------

* :ref:`type_path_result`
