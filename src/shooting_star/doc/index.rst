.. _shooting_star-index:

================================================================
 Shortest Path Shooting Star
================================================================

Function:
---------

The shortest_path_shooting_star function has the following declaration:

.. code-block:: sql

	CREATE OR REPLACE FUNCTION shortest_path_shooting_star(
						sql text, 
						source_id integer, 
						target_id integer,
						directed boolean, 
						has_reverse_cost boolean)
        RETURNS SETOF path_result


Where path_result is:

.. code-block:: sql

	CREATE TYPE path_result AS (vertex_id integer, edge_id integer, cost float8);


Arguments:
----------

**sql**: a SQL query, which should return a set of rows with the following columns:

.. code-block:: sql

	SELECT id, source, target, cost, x1, y1, x2, y2, rule, to_cost FROM edges


* id: an int4 identifier of the edge
* source: an int4 identifier of the source vertex
* target: an int4 identifier of the target vertex
* cost: double precision value of the edge traversal cost. (a negative cost will prevent the edge from being inserted in the graph).
* reverse_cost (optional): the cost for the reverse traversal of the edge. This is only used when the directed and has_reverse_cost parameters are true (see the above remark about negative costs).
* directed: true if the graph is directed
* has_reverse_cost: if true, the reverse_cost column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.
* x1: double precision value of x coordinate for edge's start vertex
* y1: double precision value of y coordinate for edge's start vertex
* x2: double precision value of x coordinate for edge's end vertex
* y2: double precision value of y coordinate for edge's end vertex
* rule: a string with a comma separated list of edge ids which describes a rule for turning restriction (if you came along these edges, you can pass through the current one only with the cost stated in to_cost column)
* to_cost: a cost of restricted passage (can be very high in a case of turn restriction or comparable with an edge cost in a case of traffic light)

**source_id**: int4 id of the start point

**directed**: true if the graph is directed

**has_reverse_cost**: if true, the reverse_cost column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.


Output:
------- 

The function returns a set of rows. There is one row for each crossed edge, and 
an additional one containing the terminal vertex. The columns of each row are:

* vertex_id: the identifier of source vertex of each edge. There is one more row after the last edge, which contains the vertex identifier of the target path.
* edge_id: the identifier of the edge crossed
* cost: The cost associated to the current edge. It is 0 for the row after the last edge. Thus, the path total cost can be computated using a sum of all rows in the cost column.


Examples:
---------

Shooting* algorithm calculates a path from edge to edge (not from vertex to 
vertex). Column vertex_id contains start vertex of an edge from column edge_id.

To describe turn restrictions:

.. code-block:: sql

	 gid | source | target | cost | x1 | y1 | x2 | y2 | to_cost | rule
	-----+--------+--------+------+----+----+----+----+---------+------
	  12 |      3 |     10 |    2 |  4 |  3 |  4 |  5 |    1000 | 14


means that the cost of going from edge 14 to edge 12 is 1000, and

.. code-block:: sql

	 gid | source | target | cost | x1 | y1 | x2 | y2 | to_cost | rule
	-----+--------+--------+------+----+----+----+----+---------+------
	  12 |      3 |     10 |    2 |  4 |  3 |  4 |  5 |    1000 | 14, 4


means that the cost of going from edge 14 to edge 12 through edge 4 is 1000.

If you need multiple restrictions for a given edge then you have to add multiple 
records for that edge each with a separate restriction. For example:

.. code-block:: sql

	 gid | source | target | cost | x1 | y1 | x2 | y2 | to_cost | rule
	-----+--------+--------+------+----+----+----+----+---------+------
	  11 |      3 |     10 |    2 |  4 |  3 |  4 |  5 |    1000 | 4
	  11 |      3 |     10 |    2 |  4 |  3 |  4 |  5 |    1000 | 12


means that the cost of going from either edge 4 or 12 to edge 11 is 1000. And 
then you always need to order your data by gid when you load it to a shortest 
path function..

To search a path using the Shooting* algorithm:

.. code-block:: sql

	SELECT * FROM shortest_path_shooting_star('SELECT id, source, target, cost, 
		     x1, y1, x2, y2, rule, to_cost FROM edges', 17, 9, true, false);


.. code-block:: sql

	 vertex_id | edge_id | cost
	-----------+---------+------
		    16 |      17 |    1
		    15 |      16 |    1
		     2 |       5 |    1
		     3 |       4 |    1
		    20 |      12 |    2
		    10 |       9 |    2
	(6 rows)


