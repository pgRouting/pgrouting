.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_tsp-V2.0:

pgr_tsp -Deprecated Signatures
===============================================================================

.. warning:: Deprecated Signatures

    This documentation is of deprecated signatures.
    
    Please use the new signatures found in :ref:`tsp`


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
