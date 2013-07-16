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
	single: pgr_tsp(sql text, start_id integer)
    single: pgr_tsp(sql text, start_id integer, end_id integer)
    single: pgr_tsp(matrix float[][], start integer)
    single: pgr_tsp(matrix float[][], start integer, end integer)
    single: pgr_makeDistanceMatrix(sqlin text)

Name
-------------------------------------------------------------------------------

* ``pgr_tsp`` - Returns the best route from a start node via a list of nodes.
* ``pgr_tsp`` - Returns the best route order when passed a disance matrix.
* ``pgr_makeDistanceMatrix`` - Returns a Eucleadian distance Matrix from the points provided in the sql result.


Synopsis
-------------------------------------------------------------------------------

The travelling salesman problem (TSP) or travelling salesperson problem asks the following question: Given a list of cities and the distances between each pair of cities, what is the shortest possible route that visits each city exactly once and returns to the origin city? This algoritm uses simulated annealing to return a high quality approximate solution. Returns a set of :ref:`pgr_costResult <type_cost_result>` (seq, id1, id2, cost) rows, that make up a path.

.. code-block:: sql

	pgr_costResult[] pgr_tsp(sql text, start_id integer);
	pgr_costResult[] pgr_tsp(sql text, start_id integer, end_id integer);


Returns a set of (seq integer, id1 integer, id2 integer, cost float8) that is the best order to visit the nodes in the matrix. ``id1`` is the index into the distance matrix. ``id2`` is the point id from the sql.

If no ``end_id`` is supplied or it is -1 or equal to the start_id then the TSP result is assumed to be a circluar loop returning back to the start. If ``end_id`` is supplied then the route is assumed to start and end the the designated ids. 

.. code-block:: sql

    record[] pgr_tsp(matrix float[][], start integer)
    record[] pgr_tsp(matrix float[][], start integer, end integer)


Description
-------------------------------------------------------------------------------

.. rubric:: With Euclidean distances

The TSP solver is based on ordering the points using straight line (euclidean) distance [#f0]_ between nodes. The implementation is using an approximation algorithm that is very fast. It is not an exact solution, but it is guaranteed that a solution is returned after certain number of iterations.

:sql: a SQL query, which should return a set of rows with the following columns:

	.. code-block:: sql

		SELECT id, x, y FROM vertex_table

	:id: ``int4`` identifier of the vertex
	:x: ``float8`` x-coordinate
	:y: ``float8`` y-coordinate

:start_id: ``int4`` id of the start point
:end_id: ``int4`` id of the end point, This is *OPTIONAL*, if include the route is optimized from start to end, otherwise it is assumed that the start and the end are the same point.


The function returns set of :ref:`type_cost_result`:

:seq:   row sequence
:id1:   internal index to the distance matric
:id2:   ``id`` of the node
:cost:  cost to traverse from the current node to the next node.

.. rubric:: Create a distance matrix

For users that need a distance matrix we have a simple function that takes SQL in ``sql`` as described above and returns a record with ``dmatrix`` and ``ids``.

    .. code-block:: sql
    
        SELECT dmatrix, ids from pgr_makeDistanceMatrix('SELECT id, x, y FROM vertex_table');

The function returns a record of ``dmatrix``, ``ids``:

:dmatrix: ``float8[][]`` a symeteric Euclidean distance matrix based on ``sql``.
:ids: ``integer[]`` an array of ids as they are ordered in the distance matrix.


.. rubric:: With distance matrix

For users, that do not want to use Euclidean distances, we also provode the ability to pass a distance matrix that we will solve and return an ordered list of nodes for the best order to visit each. It is up to the user to fully populate the distance matrix. 

:matrix: ``float[][]`` distance matrix of points
:start: ``int4`` index of the start point
:end: ``int4`` (optional) index of the end node

The ``end`` node is an optional parameter, you can just leave it out if you want a loop where the ``start`` is the depot and the route returns back to the depot. If you include the ``end`` parameter, we optimize the path from ``start`` to ``end`` and minimize the distance of the route while include the remaining points.

The distance matrix is a multidimensional `PostgreSQL array type <http://www.postgresql.org/docs/9.1/static/arrays.html>`_ that must be ``N x N`` in size. 

The result will be N records of ``[ seq, id ]``:

:seq: row sequence
:id: index into the matrix


.. rubric:: Footnotes

.. [#f0] There was some thought given to pre-calculating the driving distances between the nodes using Dijkstra, but then I read a paper (unfortunately I don't remember who wrote it), where it was proved that the quality of TSP with euclidean distance is only slightly worse than one with real distance in case of normal city layout. In case of very sparse network or rivers and bridges it becomes more inaccurate, but still wholly satisfactory. Of course it is nice to have exact solution, but this is a compromise between quality and speed (and development time also). If you need a more accurate solution, you can generate a distance matrix and use that form of the function to get your results.


.. rubric:: History

* Renamed in version 2.0.0
* GAUL dependency removed in version 2.0.0


Examples
-------------------------------------------------------------------------------

* Using SQL parameter (all points from the table, atarting from 6 and ending at 5)

.. code-block:: sql

    SELECT seq, id1, id2, round(cost::numeric, 2) AS cost
      FROM pgr_tsp('SELECT id, x, y FROM vertex_table ORDER BY id', 6, 5);

     seq | id1 | id2 | cost
    -----+-----+-----+------
       0 |   5 |   6 | 1.00
       1 |   6 |   7 | 1.00
       2 |   1 |   2 | 1.00
       3 |   0 |   1 | 1.41
       4 |   2 |   3 | 1.00
       5 |   7 |   8 | 1.41
       6 |   3 |   4 | 1.00
       7 |   8 |   9 | 1.00
       8 |  11 |  12 | 1.00
       9 |  10 |  11 | 1.00
      10 |   9 |  10 | 1.00
      11 |  12 |  13 | 2.83
      12 |   4 |   5 | 1.00
   (13 rows)


* Using distance matrix (A loop starting from 1)

.. code-block:: sql

	SELECT seq, id FROM pgr_tsp('{{0,1,3,3},{1,0,2,2},{3,2,0,2},{3,2,2,0}}'::float8[],1);

	 seq | id 
	-----+----
	   0 |  1
	   1 |  3
	   2 |  2
	   3 |  0
	(4 rows)

* Using distance matrix (Starting from 1, ending at 2)

.. code-block:: sql

	SELECT seq, id FROM pgr_tsp('{{0,1,3,3},{1,0,2,2},{3,2,0,2},{3,2,2,0}}'::float8[],1,2);

	 seq | id 
	-----+----
	   0 |  1
	   1 |  0
	   2 |  3
	   3 |  2
	(4 rows)

The queries use the :ref:`sampledata` network.


See Also
-------------------------------------------------------------------------------

* :ref:`type_cost_result`
* http://en.wikipedia.org/wiki/Traveling_salesman_problem
* http://en.wikipedia.org/wiki/Simulated_annealing
