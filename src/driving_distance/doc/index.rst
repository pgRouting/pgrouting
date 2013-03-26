:Author: Daniel Kastl
:License: Creative Commons

.. _dd:

================================================================
 Driving Distance calculation
================================================================

Function:
---------

The driving_distance function has the following signature:

.. code-block:: sql

	CREATE OR REPLACE FUNCTION driving_distance(
				sql text, 
				source_id integer, 
				distance float8, 
				directed boolean, 
				has_reverse_cost boolean) 
		RETURNS SETOF path_result


Arguments:
----------

**sql**: a SQL query, which should return a set of rows with the following columns:

* id: an int4 identifier of the edge
* source: an int4 identifier of the source vertex
* target: an int4 identifier of the target vertex
* cost: an float8 value, of the edge traversal cost. (a negative cost will prevent the edge from being inserted in the graph).

**source_id**: int4 id of the start point distance: float8 value of distance in degrees

**distance**: float8 value in edge cost units (not in projection units - they might be different).

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

.. code-block:: sql

	SELECT * FROM driving_distance('SELECT gid AS id,source,target, 
		     length::double precision AS cost FROM dourol',10549,0.01,false,false);


.. code-block:: sql

	 vertex_id | edge_id |     cost
	-----------+---------+---------------
		  6190 |  120220 | 0.00967666852
		  6205 |  118671 | 0.00961557335
		  6225 |  119384 | 0.00965668162
		  6320 |  119378 | 0.00959826176
		  ...
		  ...
		  ...
		 15144 |  122612 | 0.00973386526
		 15285 |  120471 | 0.00912965866
		 15349 |  122085 | 0.00944814966
		 15417 |  120471 | 0.00942316736
		 15483 |  121629 | 0.00972957546
	(293 rows)


