.. 
   ****************************************************************************
    pgRouting Manual
    Copyright(c) pgRouting Contributors

    This documentation is licensed under a Creative Commons Attribution-Share  
    Alike 3.0 License: http://creativecommons.org/licenses/by-sa/3.0/
   ****************************************************************************

.. _pgr_node_network:

pgr_nodeNetwork
===============================================================================

.. index:: 
	single: pgr_NodeNetwork(text,float8,text,text,text)
	module: common

Name
-------------------------------------------------------------------------------

``pgr_nodeNetwork`` - Nodes an network edge table.

:Author: Nicolas Ribot
:Copyright: Nicolas Ribot, The source code is released under the MIT-X license.


Synopsis
-------------------------------------------------------------------------------

The function reads edges from a not "noded" network table and writes the "noded" edges into a new table.

.. code-block:: sql

    text pgr_nodenetwork(text edge_table, float8, tolerance,
			 text  id='id', text the_geom='the_geom',text table_ending='noded')
  


Description
-------------------------------------------------------------------------------

A common problem associated with bringing GIS data into pgRouting is the fact that the data is often not "noded" correctly. This will create invalid topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network all the edges will be broken into separate road segments. There are cases like an over-pass and under-pass intersection where you can not traverse from the over-pass to the under-pass, but this function does not have the ability to detect and accomodate those situations.

This function reads the ``table_in`` table, that has a primary key column ``gid_cname`` and geometry column named ``geo_cname`` and intersects all the segments in it against all the other segments and then creates a table ``table_out``. It uses the ``tolerance`` for deciding that multiple nodes within the tolerance are considered the same node. 

Parameters

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:tolerance: ``float8`` tolerance for coinicident points (in projection unit)dd
:id: ``text`` Primary key column name of the network table. Default value is ``id``.
:the_geom: ``text`` Geometry column name of the network table. Default value is ``the_geom``.
:table_ending: ``text`` Suffix for the new table's. Default value is ``noded``.

The output table will have for  ``edge_table_noded``  

:id: ``bigint`` Unique identifier for the table
:old_id: ``bigint``  Identifier of the edge in original table
:sub_id: ``integer`` Segment number of the original edge
:source:
:target:
:the geom: ``geometry`` Geometry column of the noded network

.. rubric:: History

* New in version 2.0.0


Example
-------------------------------------------------------------------------------

Let's create the topology for the data in :ref:`sampledata`

.. code-block:: sql

	SELECT pgr_createTopology('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_createTopology('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Creating Topology, Please wait...
	NOTICE:  -------------> TOPOLOGY CREATED FOR  18 edges
	NOTICE:  Rows with NULL geometry or NULL id: 0
	NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
	NOTICE:  ----------------------------------------------
 	pgr_createtopology 
	--------------------
 	OK
	(1 row)
	
Now we can analyze the network.

.. code-block:: sql

	SELECT pgr_analyzegraph('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analizeGraph('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 2
	NOTICE:                            Dead ends: 7
	NOTICE:  Potential gaps found near dead ends: 1
	NOTICE:               Intersections detected: 1
	NOTICE:                      Ring geometries: 0
 	pgr_analyzegraph 
	------------------
 	OK
	(1 row)

The analysis tell us that the network has a gap and and two intersection. We try to fix the problem using:

.. code-block:: sql

	SELECT pgr_nodeNetwork('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_nodeNetwork('edge_table',0.001,'the_geom','id','noded')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Processing, pelase wait .....
	NOTICE:    Splitted Edges: 3
	NOTICE:   Untouched Edges: 15
	NOTICE:       Total original Edges: 18
	NOTICE:   Edges generated: 6
	NOTICE:   Untouched Edges: 15
	NOTICE:         Total New segments: 21
	NOTICE:   New Table: public.edge_table_noded
	NOTICE:  ----------------------------------
 	pgr_nodenetwork 
	-----------------
 	OK
	(1 row)
	
Inspecting the generated table, we can see that edges 13,14 and 18 has been segmentized

.. code-block:: sql

	SELECT old_id,sub_id FROM edge_table_noded ORDER BY old_id,sub_id;
 	old_id | sub_id 
	--------+--------
      	1      |      1
      	2      |      1
      	3      |      1
      	4      |      1
      	5      |      1
      	6      |      1
      	7      |      1
      	8      |      1
      	9      |      1
     	10     |      1
     	11     |      1
     	12     |      1
     	13     |      1
     	13     |      2
     	14     |      1
     	14     |      2
     	15     |      1
     	16     |      1
     	17     |      1
     	18     |      1
     	18     |      2
	(21 rows)
		
We can create the topology of the new network

.. code-block:: sql

	SELECT pgr_createTopology('edge_table_noded', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_createTopology('edge_table_noded',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Creating Topology, Please wait...
	NOTICE:  -------------> TOPOLOGY CREATED FOR  21 edges
	NOTICE:  Rows with NULL geometry or NULL id: 0
	NOTICE:  Vertices table for table public.edge_table_noded is: public.edge_table_noded_vertices_pgr
	NOTICE:  ----------------------------------------------
 	pgr_createtopology 
	--------------------
 	OK
	(1 row)
	
Now let's analyze the new topology

.. code-block:: sql

	SELECT pgr_analyzegraph('edge_table_noded', 0.001); 
	NOTICE:  PROCESSING:
	NOTICE:  pgr_analizeGraph('edge_table_noded',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 0
	NOTICE:                            Dead ends: 6
	NOTICE:  Potential gaps found near dead ends: 0
	NOTICE:               Intersections detected: 0
	NOTICE:                      Ring geometries: 0
 	pgr_analyzegraph 
	------------------
 	OK
	(1 row)


Images
------------

+--------------------------------------+-------------------------------------+
|.. Rubric:: Before Image              |.. Rubric:: After Image              |
|                                      |                                     |
|.. image:: images/before_node_net.png |.. image:: images/after_node_net.png |
|   :scale: 60 %                       |   :scale: 60 %                      |
|   :alt: alternate text               |   :alt: alternate text              |
|   :align: left                       |   :align: right                     |
+--------------------------------------+-------------------------------------+

Comparing the results
----------------------------------------------


Comparing with the Analysis in the original edge_table, we see that.  

+------------------+-----------------------------------------+--------------------------------------------------------------+
|                  |                Before                   |                        After                                 |
+==================+=========================================+==============================================================+
|Table name        | edge_table                              | edge_table_noded                                             |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Fields            | All original fields                     | Has only basic fields to do a topology analysis              |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Dead ends         | - Edges with 1 dead end: 1,6,24         | Edges with 1 dead end: 1-1 ,6-1,14-2, 18-1 17-1 18-2         |
|                  | - Edges with 2 dead ends  17,18         |                                                              |
|                  |                                         |                                                              |
|                  | Edge 17's right node is a dead end      |                                                              |
|                  | because there is no other edge sharing  |                                                              |
|                  | that same node. (cnt=1)                 |                                                              |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Isolated segments | two isolated segments:  17 and 18 both  | No Isolated segments                                         |
|                  | they have 2 dead ends                   |  - Edge 17 now shares a node with edges 14-1 and 14-2        |
|                  |                                         |  - Edges 18-1 and 18-2 share a node with edges 13-1 and 13-2 |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|  Gaps            | There is a gap between edge 17 and 14   | Edge 14 was segmentized                                      |
|                  | because edge 14 is near to the right    | Now edges: 14-1 14-2 17 share the same node                  |
|                  | node of edge 17                         | The tolerance value was taken in account                     |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Intersections     | Edges 13 and 18 were intersecting       | Edges were segmentized, So, now in the interection's         |
|                  |                                         | point there is a node and the following edges share it:      |
|                  |                                         | 13-1 13-2 18-1 18-2                                          |
+------------------+-----------------------------------------+--------------------------------------------------------------+










See Also
-------------------------------------------------------------------------------

:ref:`topology` for an overview of a topology for routing algorithms.
:ref:`pgr_analyze_oneway` to analyze directionality of the edges.
:ref:`pgr_create_topology`  to create a topology based on the geometry.
:ref:`pgr_analyzei_graph` to analyze the edges and vertices of the edge table.

