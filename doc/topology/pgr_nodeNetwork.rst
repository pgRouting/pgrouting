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

Name
-------------------------------------------------------------------------------

``pgr_nodeNetwork`` - Nodes an network edge table.

:Author: Nicolas Ribot
:Copyright: Nicolas Ribot, The source code is released under the MIT-X license.


Synopsis
-------------------------------------------------------------------------------

The function reads edges from a not "noded" network table and writes the "noded" edges into a new table.

.. index::
	single: pgr_NodeNetwork(Complete Signature)

.. code-block:: sql

    pgr_nodenetwork(edge_table, tolerance, id, text the_geom, table_ending, rows_where, outall)
    RETURNS TEXT


Description
-------------------------------------------------------------------------------

A common problem associated with bringing GIS data into pgRouting is the fact that the data is often not "noded" correctly. This will create invalid topologies, which will result in routes that are incorrect.

What we mean by "noded" is that at every intersection in the road network all the edges will be broken into separate road segments. There are cases like an over-pass and under-pass intersection where you can not traverse from the over-pass to the under-pass, but this function does not have the ability to detect and accommodate those situations.

This function reads the ``edge_table`` table, that has a primary key column ``id`` and geometry column named ``the_geom`` and intersect all the segments in it against all the other segments and then creates a table ``edge_table_noded``. It uses the ``tolerance`` for deciding that multiple nodes within the tolerance are considered the same node.

Parameters

:edge_table: ``text`` Network table name. (may contain the schema name as well)
:tolerance: ``float8`` tolerance for coincident points (in projection unit)dd
:id: ``text`` Primary key column name of the network table. Default value is ``id``.
:the_geom: ``text`` Geometry column name of the network table. Default value is ``the_geom``.
:table_ending: ``text`` Suffix for the new table's. Default value is ``noded``.

The output table will have for  ``edge_table_noded``

:id: ``bigint`` Unique identifier for the table
:old_id: ``bigint``  Identifier of the edge in original table
:sub_id: ``integer`` Segment number of the original edge
:source: ``integer`` Empty source column to be used with  :ref:`pgr_create_topology` function
:target: ``integer`` Empty target column to be used with  :ref:`pgr_create_topology` function
:the geom: ``geometry`` Geometry column of the noded network

.. rubric:: History

* New in version 2.0.0


Example
-------------------------------------------------------------------------------

Let's create the topology for the data in :doc:`sampledata`

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
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','true')
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

The analysis tell us that the network has a gap and an intersection. We try to fix the problem using:

.. code-block:: sql

	SELECT pgr_nodeNetwork('edge_table', 0.001);
	NOTICE:  PROCESSING:
	NOTICE:  pgr_nodeNetwork('edge_table',0.001,'the_geom','id','noded')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Processing, pelase wait .....
	NOTICE:    Split Edges: 3
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

Inspecting the generated table, we can see that edges 13,14 and 18 has been segmented

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
	NOTICE:  pgr_analyzeGraph('edge_table_noded',0.001,'the_geom','id','source','target','true')
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
 	pgr_createtopology
	--------------------
 	 OK
	(1 row)


Images
-------------------------------------------------------------------------------

.. only:: html

	+--------------------------------------+-------------------------------------+
	|.. Rubric:: Before Image              |.. Rubric:: After Image              |
	|                                      |                                     |
	|.. image:: images/before_node_net.png |.. image:: images/after_node_net.png |
	|   :scale: 60%                        |   :scale: 60%                       |
	|   :alt: before image                 |   :alt: after image                 |
	|   :align: left                       |   :align: right                     |
	+--------------------------------------+-------------------------------------+


.. only:: latex

	.. Rubric:: Before Image

	.. image:: images/before_node_net.png
		:scale: 60%
		:alt: before image
		:align: left


	.. Rubric:: After Image

	.. image:: images/after_node_net.png
		:scale: 60%
		:alt: after image
		:align: left


Comparing the results
-------------------------------------------------------------------------------

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
|Gaps              | There is a gap between edge 17 and 14   | Edge 14 was segmented                                        |
|                  | because edge 14 is near to the right    | Now edges: 14-1 14-2 17 share the same node                  |
|                  | node of edge 17                         | The tolerance value was taken in account                     |
+------------------+-----------------------------------------+--------------------------------------------------------------+
|Intersections     | Edges 13 and 18 were intersecting       | Edges were segmented, So, now in the interection's           |
|                  |                                         | point there is a node and the following edges share it:      |
|                  |                                         | 13-1 13-2 18-1 18-2                                          |
+------------------+-----------------------------------------+--------------------------------------------------------------+

Now, we are going to include the segments 13-1, 13-2 14-1, 14-2 ,18-1 and 18-2 into our edge-table, copying the data for dir,cost,and reverse cost with tho following steps:

- Add a column old_id into edge_table, this column is going to keep track the id of the original edge
- Insert only the segmented edges, that is, the ones whose max(sub_id) >1

.. code-block:: sql

	alter table edge_table drop column if exists old_id;
	alter table edge_table add column old_id integer;
	insert into edge_table (old_id,dir,cost,reverse_cost,the_geom)
   		(with
       		segmented as (select old_id,count(*) as i from edge_table_noded group by old_id)
   		select  segments.old_id,dir,cost,reverse_cost,segments.the_geom
       			from edge_table as edges join edge_table_noded as segments on (edges.id = segments.old_id)
       			where edges.id in (select old_id from segmented where i>1) );

We recreate the topology:

.. code-block:: sql

	SELECT pgr_createTopology('edge_table', 0.001);

	NOTICE:  PROCESSING:
	NOTICE:  pgr_createTopology('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait .....
	NOTICE:  Creating Topology, Please wait...
	NOTICE:  -------------> TOPOLOGY CREATED FOR  24 edges
	NOTICE:  Rows with NULL geometry or NULL id: 0
	NOTICE:  Vertices table for table public.edge_table is: public.edge_table_vertices_pgr
	NOTICE:  ----------------------------------------------
 	pgr_createtopology
	--------------------
 	OK
	(1 row)


To get the same analysis results as the topology of edge_table_noded, we do the following query:

.. code-block:: sql

	SELECT pgr_analyzegraph('edge_table', 0.001,rows_where:='id not in (select old_id from edge_table where old_id is not null)');

	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target',
                                   'id not in (select old_id from edge_table where old_id is not null)')
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
 	pgr_createtopology
	--------------------
 	OK
	(1 row)


To get the same analysis results as the original edge_table, we do the following query:

.. code-block:: sql

	SELECT pgr_analyzegraph('edge_table', 0.001,rows_where:='old_id is null')

	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','old_id is null')
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
 	pgr_createtopology
	--------------------
 	OK
	(1 row)

Or we can analyze everything because, maybe edge 18 is an overpass, edge 14 is an under pass and there is also a street level juction, and the same happens with edges 17 and 13.

.. code-block:: sql

	SELECT pgr_analyzegraph('edge_table', 0.001);

	NOTICE:  PROCESSING:
	NOTICE:  pgr_analyzeGraph('edge_table',0.001,'the_geom','id','source','target','true')
	NOTICE:  Performing checks, pelase wait...
	NOTICE:  Analyzing for dead ends. Please wait...
	NOTICE:  Analyzing for gaps. Please wait...
	NOTICE:  Analyzing for isolated edges. Please wait...
	NOTICE:  Analyzing for ring geometries. Please wait...
	NOTICE:  Analyzing for intersections. Please wait...
	NOTICE:              ANALYSIS RESULTS FOR SELECTED EDGES:
	NOTICE:                    Isolated segments: 0
	NOTICE:                            Dead ends: 3
	NOTICE:  Potential gaps found near dead ends: 0
	NOTICE:               Intersections detected: 5
	NOTICE:                      Ring geometries: 0
 	pgr_createtopology
	--------------------
 	OK
	(1 row)


See Also
-------------------------------------------------------------------------------

:ref:`topology` for an overview of a topology for routing algorithms.
:ref:`pgr_analyze_oneway` to analyze directionality of the edges.
:ref:`pgr_create_topology` to create a topology based on the geometry.
:ref:`pgr_analyze_graph` to analyze the edges and vertices of the edge table.

.. rubric:: Indices and tables

* :ref:`genindex`
* :ref:`search`
