:Author: Daniel Kastl
:License: Creative Commons

.. _tsp:

================================================================
  Traveling Sales Person (TSP)
================================================================

The tsp solution is based on ordering the points using straight line (euclidean) 
distance between nodes. There was some thought given to pre-calculating the 
driving distances between the nodes using Dijkstra, but then I read a paper 
(unfortunately I don't remember who wrote it), where it was proved that the 
quality of TSP with euclidean distance is only slightly worse that one with real 
distance in case of normal city layout. In case of very sparse network or rivers 
and bridges it becomes more inaccurate, but still wholly satisfactory. Of course 
it is nice to have exact solution, but this is a compromise between quality and 
speed (and development time also).

The TSP solver is using a genetic algorithm. It is not an exact solution, but it 
is guarantied that you get a solution after certain number of iterations.

Function:
---------

The pgpsql tsp function has the following signature:

.. code-block:: sql

	CREATE OR REPLACE FUNCTION tsp(
				sql text, 
				ids varchar, 
				source_id integer)
		RETURNS SETOF path_result


Arguments:
----------

**sql**: a SQL query, which should return a set of rows with the following columns:

* source_id: an int4 identifier of the vertex
* x: x coordinate of the vertex
* y: y coordinate of the vertex

**ids**: text string containig int4 ids of vertices separated by commas

**source_id**: int 4 id of the start point

Output:
------- 

The function returns a set of rows. There is one row for each crossed edge, and 
an additional one containing the terminal vertex. The columns of each row are:

* vertex_id: the identifier of source vertex of each edge. There is one more row after the last edge, which contains the vertex identifier of the target path.
* edge_id: unused, always 0
* cost: unused, always 0

Examples:
---------

.. code-block:: sql

	SELECT * FROM tsp('SELECT distinct source AS source_id, 
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
	

Afterwards vertex_id column can be used for shortest path calculation.

