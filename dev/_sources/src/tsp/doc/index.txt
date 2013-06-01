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

* ``pgr_tsp`` - Returns the best route from a start node via a list of nodes.
* ``pgr_tsp`` - Returns the best route order when passed a disance matrix.


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

.. rubric:: With Euclidean distances

The TSP solver is based on ordering the points using straight line (euclidean) distance [#f1]_ between nodes. The implementation is using a genetic algorithm. It is not an exact solution, but it is guaranteed that a solution is returned after certain number of iterations.

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT source_id, x, y FROM vertex_table

	:source_id: ``int4`` identifier of the vertex
	:x: ``float8`` x-coordinate
	:y: ``float8`` y-coordinate

:ids: ``text`` string containig ``int4`` ids of vertices separated by commas
:source: ``int4`` id of the start point


The function returns set of :ref:`type_cost_result` [#f0]_:

:seq:   row sequence
:id1:   ...
:id2:   ...
:cost:  cost to traverse the path from ``id1`` to ``id2``. Cost will be -1.0 if there is no path to that target vertex id.

.. rubric:: With distance matrix

For users, that do not want to use Euclidean distances, we also provode the ability to pass a distance matrix that we will solve and return an ordered list of nodes for the best order to visit each. It is up to the user to fully populate the distance matrix. 

:matrix: ``float[][]`` distance matrix of points
:start: ``int4`` index of the start point

The distance matrix is a multidimensional `PostgreSQL array type <http://www.postgresql.org/docs/9.1/static/arrays.html>`_ that must be ``N x N`` in size. 

The result will be N records of ``[ seq, id ]``:

:seq: row sequence
:id: index into the matrix


.. rubric:: Footnotes

.. [#f0] ``edge_id`` and ``cost`` attribute of the result set are not used and always contain ``0``.
.. [#f1] There was some thought given to pre-calculating the driving distances between the nodes using Dijkstra, but then I read a paper (unfortunately I don't remember who wrote it), where it was proved that the quality of TSP with euclidean distance is only slightly worse that one with real distance in case of normal city layout. In case of very sparse network or rivers and bridges it becomes more inaccurate, but still wholly satisfactory. Of course it is nice to have exact solution, but this is a compromise between quality and speed (and development time also).


.. rubric:: History

* Renamed in version 2.0.0
* GAUL dependency removed in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Using SQL parameter

.. code-block:: sql

	SELECT * FROM pgr_tsp('SELECT id AS source_id, x, y FROM vertex_table','2,7,11',7);


* Using distance matrix

.. code-block:: sql

	SELECT seq, id FROM pgr_tsp('{{0,1,2,3},{1,0,3,2},{2,3,0,4},{3,2,4,0}}',2);

	 seq | id 
	-----+----
	   0 |  2
	   1 |  0
	   2 |  1
	   3 |  3
	(4 rows)


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
