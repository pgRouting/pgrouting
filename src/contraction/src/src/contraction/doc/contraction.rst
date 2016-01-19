.. _shooting_star-index:

================================================================
 Contraction Hierarchies
================================================================

Introduction:
-------------

Contraction Hierarchies reduce the size of the graph by removing some of the vertices and edges(according to a priority)and adds some psuedo edges,such that the number of edges are reduced on a whole.This reduces the time and space complexity of many algorithms that make various operations on the graph.



Function:
---------

The pgr_contractgraph function has the following declaration:

.. code-block:: sql

	CREATE OR REPLACE FUNCTION pgr_contractgraph(
						sql text, 
						level integer,
						has_rcost boolean)
        RETURNS SETOF pgr_contracted_blob


Where pgr_contracted_blob is:

.. code-block:: sql

	CREATE TYPE pgr_contracted_blob AS
 (
 	contracted_graph_name text,
 	contracted_graph_blob text,
 	removedVertices text,
 	removedEdges text,
 	psuedoEdges text
 );


Arguments:
----------

**sql**: a SQL query, which should return a set of rows with the following columns:

.. code-block:: sql

	SELECT id, source, target, cost, revcost FROM edges


* id: an int4 identifier of the edge
* source: an int4 identifier of the source vertex
* target: an int4 identifier of the target vertex
* cost: double precision value of the edge traversal cost. (a negative cost will prevent the edge from being inserted in the graph).
* revcost: the cost for the reverse traversal of the edge. This is only used when the directed and has_reverse_cost parameters are true (see the above remark about negative costs).

**level**: an integer which defines the level of contraction.

**has_rcost**: if true, the reverse_cost column of the SQL generated set of rows will be used for the cost of the traversal of the edge in the opposite direction.

Output:
------- 

The function returns a single row. The columns of the row are:

* contracted_graph_name: name of the contracted graph.
* contracted_graph_blob: edges of the contracted graph in text format.
* removedVertices      : vertices removed during contraction. 
* removedEdges		   : edges removed during contraction.
* psuedoEdges		   : shortcuts introduced during contraction.


Examples:
---------

Each row in the edge table should be of this format

.. code-block:: sql

	 gid | source | target | cost | reverse_cost 
	-----+--------+--------+------+--------------
	  12 |      3 |     10 |    2 |  	-1 


means that the cost of going from edge 3 to edge 10 is 2.

Let us consider the edge table of a sample graph,and generate a contracted version of it.
 	
.. code-block:: sql

 	 id  | source   | target | cost | reverse_cost 
    -----+----------+--------+------+-------------
       1 |        1 |    2 	 |    1 |   	-1  
       2 |        2 |    3   |    2 |    	 3
       3 |        2 |    4   |    1 |    	 1   
       4 |        3 |    4   |    3 |   	-1 
       5 |        4 |    5   |    1 |    	 2   
       6 |        5 |    6   |    4 |   	-1   


To contract a graph:

Level 0:

	SELECT * FROM pgr_contractgraph('SELECT id, source, target, cost, 
		     reverse_cost as revcost FROM edges',0,false);

.. code-block:: sql

	contracted_graph_name | contracted_graph_blob           | removedvertices                      |           removededges             | psuedoedges 
	----------------------+---------------------------------+--------------------------------------+------------------------------------+-------------
 	 contracted_graph_0   | 2,2,3,2,0$3,2,4,1,0$4,3,4,3,0$  | 1,1,1,2,1,0$5,5,4,5,1,0$6,6,5,6,4,0$ |  1,1,2,1,-1$5,5,4,1,-1$6,6,5,4,-1$ | 

	(1 row)


Level 1:

	SELECT * FROM pgr_contractgraph('SELECT id, source, target, cost, 
		     reverse_cost as revcost FROM edges',1,false);

.. code-block:: sql

	contracted_graph_name | contracted_graph_blob           | removedvertices                                              |         removededges                                     | psuedoedges 
	----------------------+---------------------------------+--------------------------------------------------------------+----------------------------------------------------------+---------------------
 	 contracted_graph_1   |           3,2,4,1,0$            | 1,1,1,2,1,0$2,4,3,4,3,0$2,2,2,3,2,0$5,5,4,5,1,0$6,6,5,6,4,0$ |  1,1,2,1,-1$2,3,2,2,-1$4,3,4,3,-1$5,5,4,1,-1$6,6,5,4,-1$ | $3,2,4


	(1 row)




