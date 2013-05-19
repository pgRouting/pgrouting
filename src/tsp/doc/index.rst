.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_tsp:

pgr_tsp - Traveling Sales Person
===============================================================================

.. index:: 
	single: pgr_tsp(text, integer, double precision, boolean, boolean)
    single: pgr_tsp(matrix float[][], start integer)

Name
-------------------------------------------------------------------------------

``pgr_tsp`` - Returns the best route from a start node via a list of nodes.
``pgr_tsp`` - Returns the best order when passed a disance matrix.


Synopsis
-------------------------------------------------------------------------------

Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_tsp(sql text, ids varchar, source integer);

Resturns a set of (seq integer, id integer) that is the best order to visit the nodes in the matrix.

.. code-block:: sql

    record[] pgr_tsp(matrix float[][], start integer)

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

For users, that do not want to use Euclidean distances, we also provode the
ability to pass a distance matrix that we will solve and return an ordered
list of nodes for the best order to visit each. It is up to the user to
fully populate the distance matrix. The distance matrix is a multidimensional
postgresql array type that must be N x N in size. The result will be N records
of ``[ seq, id ]`` where ``id`` is an index into the matrix. See the PostgreSQL
documentation of ARRAY types, functions and array_agg.


.. rubric:: History

* Renamed in version 2.0.0
* GAUL dependency removed in version 2.0.0


Examples
-------------------------------------------------------------------------------

.. code-block:: sql

	SELECT * FROM pgr_tsp('SELECT distinct source, 
		               x1::double precision AS x, 
		               y1::double precision AS y FROM edge_table 
		     WHERE source IN (<list of ids>)',
		               '<list of ids>', <start id>);

    SELECT seq, id from pgr_tsp('{{0,1,2,3},{1,0,3,2},{2,3,0,4},{3,2,4,0}}', 2);


.. note::
	Afterwards ``vertex_id`` column can be used for shortest path calculation.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
