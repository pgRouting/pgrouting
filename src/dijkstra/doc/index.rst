.. _dijkstra-index:

================================================================
 Shortest Path Dijkstra
================================================================

Function:
---------

The shortest_path function has the following declaration:

.. code-block:: sql

	CREATE OR REPLACE FUNCTION shortest_path(
							sql text, 
							source_id integer,
							target_id integer, 
							directed boolean, 
							has_reverse_cost boolean) 
		RETURNS SETOF path_result


Arguments:
----------

**sql**: a SQL query, which should return a set of rows with the following columns:

.. code-block:: sql

	SELECT id, source, target, cost FROM edge_table


* id: an int4 identifier of the edge
* source: an int4 identifier of the source vertex
* target: an int4 identifier of the target vertex
* cost: an float8 value, of the edge traversal cost. (a negative cost will prevent the edge from being inserted in the graph).
* reverse_cost (optional): the cost for the reverse traversal of the edge. This is only used when the directed and has_reverse_cost parameters are true (see the above remark about negative costs).

**source_id**: int4 id of the start point

**directed**: true if the graph is directed

**has_reverse_cost**: if true, the reverse_cost column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Output:
------- 

The function returns a set of rows. There is one row for each crossed edge, and an additional one containing the terminal vertex. The columns of each row are:

* vertex_id: the identifier of source vertex of each edge. There is one more row after the last edge, which contains the vertex identifier of the target path.
* edge_id: the identifier of the edge crossed
* cost: The cost associated to the current edge. It is 0 for the row after the last edge. Thus, the path total cost can be computated using a sum of all rows in the cost column.


Examples:
---------

.. code-block:: sql

	SELECT * FROM shortest_path('SELECT gid AS id, source::int4, 
		     target::int4, length::double precision AS cost,
		FROM dourol',3, 7, false, false);


.. code-block:: sql

	 vertex_id | edge_id | cost 
	-----------+---------+------------------------
		     3 |       2 |    0.000763954363701041
		     4 |      21 |    0.00150254971056274
		     6 |       5 |    0.000417442425988342
		     7 |      -1 |    0
	(4 rows)


.. code-block:: sql

	SELECT * FROM shortest_path('SELECT gid AS id, source::int4, 
		     target::int4, length::double precision AS cost,length::double precision 
		AS reverse_cost FROM dourol', 3, 7, true, true);



